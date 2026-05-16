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
 * @file        forward_pass.cpp
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       ForwardPassData implementation and pass-builder free functions
 */
#include "smpch.h"
#include "forward_pass.h"

#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/renderer/material/material_system.h"
#include "smile/graphic/renderer/render_graph/render_graph.h"
#include "smile/graphic/rhi/format.h"
#include "smile/core/math/color.h"

namespace smile::graphic
{
    // ---- ForwardPassData --------------------------------------------------------

    void ForwardPassData::Initialize( RenderContext &context,
        ResourceManager &resourceManager,
        MaterialSystem &materialSystem )
    {
        Context = &context;
        ResourceMgr = &resourceManager;
        MaterialSys = &materialSystem;

        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );
            cameraCBDesc.Add( "ViewInverse", ConstantType::Mat4 );
            CameraCB = resourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            ConstantBufferDescriptor perObjectCBDesc{};
            perObjectCBDesc.Add( "World", ConstantType::Mat4 );
            PerObjectCB = resourceManager.CreateConstantBuffer( perObjectCBDesc );
        }

        {
            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, CameraCB.GetHandle() ) },
                { rhi::BindingSetElement::CreateConstantBuffer( 1, PerObjectCB.GetHandle() ) } };

            resourceManager.CreateBindingSetAndLayout(
                bindingSetDesc, { rhi::ShaderStage::Vertex }, PassBindingLayout, PassBindingSet );
        }
    }

    void ForwardPassData::ShutDown()
    {
        Pipelines.Clear();
        Context = nullptr;
        ResourceMgr = nullptr;
        MaterialSys = nullptr;
    }

    void ForwardPassData::SetupMaterial( MaterialInstance materialInstance, GraphicsState &graphicsState )
    {
        MaterialSys->UpdateMaterialInstance( materialInstance );

        auto it = Pipelines.FindItemAtKey( materialInstance );
        if ( it == Pipelines.end() )
        {
            it = CreatePipeline( materialInstance );
        }

        graphicsState.Pipeline = it.GetItem();

        const auto &materialData = MaterialSys->GetMaterialData( materialInstance );
        graphicsState.Bindings.PushBack( materialData.Bindings );
    }

    primitive::HashMap< MaterialInstance, GraphicsPipeline >::Iterator ForwardPassData::CreatePipeline(
        MaterialInstance materialInstance )
    {
        const auto &materialData = MaterialSys->GetMaterialData( materialInstance );
        const auto material = materialInstance.GetMaterial();

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

        psoDesc.VertexShader = ResourceMgr->GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );
        psoDesc.PixelShader = ResourceMgr->GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        psoDesc.BindingLayouts.PushBack( PassBindingLayout );
        psoDesc.BindingLayouts.PushBack( materialData.BindingLayout );

        psoDesc.RenderState = material.GetLayout().RenderState;

        auto pipeline = ResourceMgr->CreateGraphicsPipeline( psoDesc );
        return Pipelines.Insert( materialInstance, std::move( pipeline ) );
    }

    // ---- Internal helper --------------------------------------------------------

    static void DrawRenderables( ForwardPassData &data,
        const Framebuffer &framebuffer,
        const primitive::Vector< Renderable > &renderables )
    {
        data.Context->FillConstantBuffer( data.CameraCB );

        for ( const Renderable &renderable : renderables )
        {
            data.PerObjectCB.Update( &renderable.GetWorldTransform() );
            data.Context->FillConstantBuffer( data.PerObjectCB );

            for ( const RenderPrimitive &primitive : renderable )
            {
                GraphicsState state{};
                state.Framebuffer = framebuffer;
                state.VertexBuffers.PushBack( { primitive.VertexBuffer, 0u, 0u } );
                state.IndexBuffer = IndexBufferBinding{ primitive.IndexBuffer, rhi::Format::R32_UINT, 0u };
                state.Bindings.PushBack( data.PassBindingSet );

                data.SetupMaterial( primitive.MaterialInstance, state );

                data.Context->SetGraphicsState( state );
                data.Context->DrawIndexed( primitive.IndexBuffer.GetIndexCount() );
            }
        }
    }

    // ---- Pass-builder free functions --------------------------------------------

    void AddForwardPass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        const Uint32 width,
        const Uint32 height,
        RenderGraphResourceHandle &outColor,
        RenderGraphResourceHandle &outDepth )
    {
        graph.AddPass(
            "ForwardPass",
            [&, width, height]( RenderGraphPassBuilder &builder )
            {
                outColor = builder.CreateTexture(
                    "SceneColor", RenderGraphTextureDescriptor{ width, height, rhi::Format::RGBA8_UNORM } );

                outDepth = builder.CreateTexture(
                    "SceneDepth", RenderGraphTextureDescriptor{ width, height, rhi::Format::D24S8 } );

                builder.WriteColor( outColor );
                builder.WriteDepth( outDepth );
            },
            [&data, &scene]( const RenderGraphPassResources &res, RenderContext & )
            {
                const Framebuffer &fb = res.GetFramebuffer();
                data.Context->Clear( data.ResourceMgr->GetFramebufferAttachmentSet( fb ),
                    math::Color{ 0.0f, 0.0f, 0.0f, 1.0f },
                    1.0f,
                    0 );

                scene.GetView().FillConstants( data.ViewCons );
                data.CameraCB.Update( &data.ViewCons );
                DrawRenderables( data, fb, scene.GetRenderables( SceneLayer::World ) );
            } );
    }

    void AddSpritePass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        RenderGraphResourceHandle &inOutColor,
        RenderGraphResourceHandle &inOutDepth )
    {
        graph.AddPass(
            "SpritePass",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( inOutDepth );
                builder.WriteColor( inOutColor );
                builder.WriteDepth( inOutDepth );
            },
            [&data, &scene]( const RenderGraphPassResources &res, RenderContext & )
            { DrawRenderables( data, res.GetFramebuffer(), scene.GetRenderables( SceneLayer::Sprite ) ); } );
    }

    void AddUIPass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        RenderGraphResourceHandle &inOutColor )
    {
        graph.AddPass(
            "UIPass",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( inOutColor );
                builder.WriteColor( inOutColor );
            },
            [&data, &scene]( const RenderGraphPassResources &res, RenderContext & )
            { DrawRenderables( data, res.GetFramebuffer(), scene.GetRenderables( SceneLayer::UI ) ); } );
    }
}
