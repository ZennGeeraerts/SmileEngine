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
#include "render_world.h"
#include "smile/graphic/renderer/draw/draw_command_buffer.h"
#include "smile/graphic/renderer/graphics_state.h"
#include "smile/graphic/renderer/mesh/mesh.h"
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
    }

    void ForwardPassData::ShutDown()
    {
        Context = nullptr;
        ResourceMgr = nullptr;
        MaterialSys = nullptr;
    }

    // ---- Pass-builder free functions --------------------------------------------

    void AddForwardPass( RenderGraph &graph,
        ForwardPassData &data,
        RenderWorld &renderWorld,
        const DrawCommandBuffer &buffer,
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
            [&data, &renderWorld, &buffer]( const RenderGraphPassResources &res, RenderContext & )
            {
                const Framebuffer &fb = res.GetFramebuffer();
                data.Context->Clear( data.ResourceMgr->GetFramebufferAttachmentSet( fb ),
                    math::Color{ 0.0f, 0.0f, 0.0f, 1.0f },
                    1.0f,
                    0 );

                for ( DrawBin *bin : buffer.GetBins() )
                {
                    for ( const ecs::EntityHandle entity : bin->Entities )
                    {
                        const auto &mesh = renderWorld.GetComponent< Mesh >( entity );
                        const auto &mi = renderWorld.GetComponent< MaterialInstance >( entity );
                        const auto &pipeline = renderWorld.GetComponent< GraphicsPipeline >( entity );

                        data.MaterialSys->UpdateMaterialInstance( mi );
                        const auto &materialData = data.MaterialSys->GetMaterialData( mi );

                        GraphicsState state{};
                        state.Framebuffer = fb;
                        state.Pipeline = pipeline;
                        state.VertexBuffers.PushBack( { mesh.VertexBuffer, 0u, 0u } );
                        state.IndexBuffer = IndexBufferBinding{ mesh.IndexBuffer, rhi::Format::R32_UINT, 0u };
                        state.Bindings.PushBack( renderWorld.GetPassBindingSet() );
                        state.Bindings.PushBack( materialData.Bindings );

                        data.Context->SetGraphicsState( state );
                        data.Context->DrawIndexed( mesh.IndexBuffer.GetIndexCount() );
                    }
                }
            } );
    }
}
