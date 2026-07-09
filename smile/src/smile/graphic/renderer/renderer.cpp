/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        renderer.cpp
 * @author      Zenn Geeraerts
 * @created     17 April 2026
 * @brief       Per-frame orchestration — builds the RenderGraph each frame
 */
#include "smpch.h"
#include "renderer.h"

#include "frame.h"
#include "render_engine.h"
#include "render_world.h"
#include "draw/binned_command_buffer.h"
#include "smile/core/math/color.h"
#include "smile/core/world/ecs/transform_component.h"
#include "smile/graphic/renderer/render_graph/render_graph_resource.h"
#include "smile/graphic/renderer/view.h"

namespace smile::graphic
{
    static rhi::GPUBufferDescriptor CreateInstanceBufferDesc()
    {
        rhi::GPUBufferDescriptor desc{};
        desc.Usage = rhi::BufferUsage::Dynamic;
        desc.CPUAccess = rhi::CPUAccessMode::Write;
        desc.BindFlags.Set( rhi::BufferBindFlags::VertexBuffer );

        return desc;
    }

    Renderer::Renderer( RenderEngine &engine ) noexcept
        : m_Engine{ engine },
          m_Graph{ engine.GetResourceManager() },
          m_BufferAllocator{ engine.GetResourceManager(), CreateInstanceBufferDesc() }
    {
        auto &ctx = engine.GetRenderContext();
        auto &rm = engine.GetResourceManager();

        m_DebugData.Initialize( ctx, rm, engine.GetShaderLibrary() );
        m_BufferAllocator.Initialize( 16 * 1024 * 1024 ); // TODO: Move to separate method
    }

    void Renderer::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
        m_Engine.GetRenderContext().Open();
    }

    void Renderer::OnRender( RenderWorld &renderWorld )
    {
        auto &ctx = m_Engine.GetRenderContext();
        auto &resourceManager = m_Engine.GetResourceManager();

        renderWorld.Prepare( ctx, resourceManager, m_Engine.GetMeshManager(), m_Engine.GetMaterialSystem() );
        renderWorld.Enqueue( m_Opaque3dCommandBuffers, m_Transparent3dCommandBuffers );

        m_DebugRenderer.Flush( m_DebugData ); // DebugPassData::LineList

        auto group = renderWorld.GetGroup< View >();
        for ( const auto viewEntity : group )
        {
            BuildRenderGraph( renderWorld, viewEntity, resourceManager );
        }

        m_Graph.Compile();
        m_Graph.Execute( ctx );
    }

    void Renderer::RenderView( const RenderWorld &renderWorld,
        const ecs::EntityHandle viewEntity,
        RenderContext &ctx,
        ResourceManager &resourceManager )
    {
        BuildRenderGraph( renderWorld, viewEntity, resourceManager );

        m_Graph.Compile();
        m_Graph.Execute( ctx );
    }

    void Renderer::BuildRenderGraph( const RenderWorld &renderWorld,
        const ecs::EntityHandle viewEntity,
        ResourceManager &resourceManager )
    {
        // Register render passes
        RenderGraphResourceHandle colorHandle;
        RenderGraphResourceHandle depthHandle;

        const auto &view = renderWorld.GetComponent< View >( viewEntity );
        const auto &renderTarget = view.GetRenderTarget();

        m_Graph.AddPass(
            "OpaquePass",
            [&]( RenderGraphPassBuilder &builder )
            {
                const auto width = renderTarget.GetWidth();
                const auto height = renderTarget.GetHeight();

                colorHandle = builder.CreateTexture(
                    "SceneColor", RenderGraphTextureDescriptor{ width, height, rhi::Format::RGBA8_UNORM } );

                depthHandle = builder.CreateTexture(
                    "SceneDepth", RenderGraphTextureDescriptor{ width, height, rhi::Format::D24S8 } );

                builder.WriteColor( colorHandle );
                builder.WriteDepth( depthHandle );
            },
            [this, &renderWorld, &resourceManager, viewEntity](
                const RenderGraphPassResources &res, RenderContext &ctx )
            {
                const Framebuffer &fb = res.GetFramebuffer();
                ctx.Clear(
                    resourceManager.GetFramebufferAttachmentSet( fb ), math::Color{ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f, 0 );

                const auto &viewBindingSet = renderWorld.GetComponent< BindingSet >( viewEntity );
                const auto &viewBindingLayout = renderWorld.GetComponent< BindingLayout >( viewEntity );
                const auto &cmdBuffer = m_Opaque3dCommandBuffers[viewEntity];

                for ( const DrawBin< Opaque3d > *bin : cmdBuffer.GetSorted() )
                {
                    const Count instanceCount = bin->GetInstanceCount();
                    const Uint32 instanceStride = sizeof( DirectX::XMFLOAT4X4 );

                    BufferSlice instanceDataSlice =
                        m_BufferAllocator.Allocate( instanceCount * instanceStride, alignof( DirectX::XMFLOAT4X4 ) );

                    VertexBuffer instanceBuffer{ instanceDataSlice.Buffer, instanceCount, instanceStride };

                    primitive::Vector< DirectX::XMFLOAT4X4 > instanceData( instanceCount );
                    for ( Index index{ 0 }; index < instanceCount; ++index )
                    {
                        auto entity = bin->Items[index].GetEntity();

                        const auto &transform = renderWorld.GetComponent< world::ecs::TransformComponent >( entity );
                        const auto worldTransform = transform.GetWorldTransform();

                        instanceData[index] = worldTransform;
                    }

                    ctx.FillVertexBuffer( instanceBuffer, instanceData.GetData(), instanceCount );

                    const auto &item = bin->Items[0];
                    const auto entity = item.GetEntity();
                    const auto &mesh = renderWorld.GetComponent< Mesh >( entity );
                    const auto &mi = renderWorld.GetComponent< MaterialInstance >( entity );
                    const auto psoDescHandle = renderWorld.GetComponent< GraphicsPipelineDescriptorHandle >( entity );
                    const auto &psoDesc = resourceManager.GetGraphicsPipelineDescriptor( psoDescHandle );
                    const auto pipeline = resourceManager.GetOrCreateGraphicsPipeline( psoDesc, fb );

                    const auto &materialData = m_Engine.GetMaterialSystem().GetMaterialData( mi );

                    GraphicsState state{};
                    state.Framebuffer = fb;
                    state.Pipeline = pipeline;
                    state.VertexBuffers.PushBack( { mesh.VertexBuffer, 0u, 0u } );
                    state.VertexBuffers.PushBack( { instanceBuffer, 1u, instanceDataSlice.Range.Offset } );
                    state.IndexBuffer = IndexBufferBinding{ mesh.IndexBuffer, rhi::Format::R32_UINT, 0u };
                    state.Bindings.PushBack( viewBindingSet );
                    state.Bindings.PushBack( materialData.Bindings );

                    ctx.SetGraphicsState( state );
                    ctx.DrawInstancedIndexed( mesh.IndexBuffer.GetIndexCount(), instanceCount );
                }
            } );

        AddDebugPass( m_Graph, m_DebugData, view, colorHandle );

        // PostProcess - extension point (tone-map, bloom); currently a no-op passthrough
        m_Graph.AddPass(
            "PostProcess",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( colorHandle );
                builder.WriteColor( colorHandle );
            },
            []( const RenderGraphPassResources &, RenderContext & ) {} );

        // PresentPass - blit SceneColor to final render target
        m_Graph.AddPass(
            "PresentPass",
            [&]( RenderGraphPassBuilder &builder ) { builder.ReadTexture( colorHandle ); },
            [&resourceManager, colorHandle, &renderWorld, &renderTarget](
                const RenderGraphPassResources &res, RenderContext &ctx )
            {
                const FramebufferAttachmentSet &fbSet = resourceManager.GetFramebufferAttachmentSet( renderTarget );

                SM_ASSERT( !fbSet.ColorAttachments.IsEmpty() );

                ctx.CopyTexture( fbSet.ColorAttachments[0].Texture, {}, res.GetTexture( colorHandle ), {} );
            } );
    }

    void Renderer::EndFrame( const rhi::SwapChain &swapChain )
    {
        auto &ctx = m_Engine.GetRenderContext();

        swapChain.Present();
        ctx.Close();

        m_Opaque3dCommandBuffers.Clear();
        m_Transparent3dCommandBuffers.Clear();

        m_Graph.Reset();
        m_DebugData.Reset();
        m_DebugRenderer.Reset();
        m_BufferAllocator.Reset();

        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }
}
