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
 * @file        debug_pass.cpp
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       DebugPassData implementation and AddDebugPass builder function
 */
#include "smpch.h"
#include "debug_pass.h"

#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/render_scene.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/common/foundation/numeric_cast.h"
#include "smile/graphic/shader/shader_library.h"
#include "smile/graphic/shader/shader_asset.h"
#include "smile/graphic/renderer/shader/program.h"
#include "smile/graphic/renderer/render_graph/render_graph.h"
#include "smile/graphic/renderer/shader/binding_layout.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    // ---- DebugPassData ----------------------------------------------------------

    void DebugPassData::Initialize( RenderContext &context,
        ResourceManager &resourceManager,
        const ShaderLibrary &shaderLib )
    {
        Context = &context;
        ResourceMgr = &resourceManager;

        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );
            CameraCB = resourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            auto vertexShaderAsset = shaderLib.Get( "debug_renderer.vs" );
            auto pixelShaderAsset = shaderLib.Get( "pos_col.ps" );

            // Validates VS/PS shader-pair compatibility (input/output signature, CB layout) at startup.
            // The returned program is not stored, the debug pipeline uses an explicit vertex layout
            // rather than VS reflection.
            [[maybe_unused]] const auto debugProgram = Program::Create( vertexShaderAsset, pixelShaderAsset );

            GraphicsPipelineDescriptor psoDesc{};
            psoDesc.Topology = rhi::PrimitiveTopology::LineList;
            psoDesc.InputLayout = m_VertexLayout;
            psoDesc.VertexShader = resourceManager.CreateVertexShader( vertexShaderAsset );
            psoDesc.PixelShader = resourceManager.CreatePixelShader( pixelShaderAsset );

            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, CameraCB.GetHandle() ) } };

            BindingLayout bindingLayout;
            resourceManager.CreateBindingSetAndLayout(
                bindingSetDesc, { rhi::ShaderStage::Vertex }, bindingLayout, PassBindingSet );

            psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );
            psoDesc.RenderState.RasterizerState.CullMode = rhi::CullMode::None;

            Pipeline = resourceManager.CreateGraphicsPipeline( psoDesc );
        }

        DynamicVB = resourceManager.CreateDynamicVertexBuffer( VertexCapacity, m_VertexLayout );
    }

    void DebugPassData::ShutDown()
    {
        LineList.Clear();
        Context = nullptr;
        ResourceMgr = nullptr;
    }

    void DebugPassData::Reset()
    {
        LineList.Clear();
    }

    void DebugPassData::GrowVertexBuffer()
    {
        ResourceMgr->DestroyVertexBuffer( DynamicVB );
        DynamicVB = ResourceMgr->CreateDynamicVertexBuffer( VertexCapacity, m_VertexLayout );
    }

    void DebugPassData::CreateFixedLineList()
    {
        constexpr Uint32 numGridLines = 20;
        constexpr float gridSpacing = 1.0f;

        const float startOffset = -static_cast< float >( numGridLines / 2 ) * gridSpacing;
        const float size = ( numGridLines - 1 ) * gridSpacing;
        const auto gridColor = static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::LightGray );

        for ( Uint32 i = 0; i < numGridLines; ++i )
        {
            const float lineOffset = startOffset + gridSpacing * static_cast< float >( i );

            // Vertical line (along X)
            DirectX::XMFLOAT3 a{ startOffset, 0.0f, lineOffset };
            DirectX::XMFLOAT3 b{ startOffset + size, 0.0f, lineOffset };
            LineList.PushBack( { a, gridColor } );
            LineList.PushBack( { b, gridColor } );

            // Horizontal line (along Z)
            a = { lineOffset, 0.0f, startOffset };
            b = { lineOffset, 0.0f, startOffset + size };
            LineList.PushBack( { a, gridColor } );
            LineList.PushBack( { b, gridColor } );
        }

        // Axis lines
        const auto red = static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed );
        const auto green = static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen );
        const auto blue = static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue );

        LineList.PushBack( { { 0.0f, 0.0f, 0.0f }, red } );
        LineList.PushBack( { { 30.0f, 0.0f, 0.0f }, red } );
        LineList.PushBack( { { 0.0f, 0.0f, 0.0f }, green } );
        LineList.PushBack( { { 0.0f, 30.0f, 0.0f }, green } );
        LineList.PushBack( { { 0.0f, 0.0f, 0.0f }, blue } );
        LineList.PushBack( { { 0.0f, 0.0f, 30.0f }, blue } );
    }

    void DebugPassData::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &color )
    {
        LineList.PushBack( { start, color } );
        LineList.PushBack( { end, color } );
    }

    void DebugPassData::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &colorStart,
        const DirectX::XMFLOAT4 &colorEnd )
    {
        LineList.PushBack( { start, colorStart } );
        LineList.PushBack( { end, colorEnd } );
    }

    // ---- AddDebugPass -----------------------------------------------------------

    void AddDebugPass( RenderGraph &graph,
        DebugPassData &data,
        const RenderScene &scene,
        RenderGraphResourceHandle &inOutColor )
    {
        graph.AddPass(
            "DebugPass",
            [&]( RenderGraphPassBuilder &builder )
            {
                builder.ReadTexture( inOutColor );
                builder.WriteColor( inOutColor );
            },
            [&data, &scene]( const RenderGraphPassResources &res, RenderContext & )
            {
                // Prepend fixed grid + axes
                data.CreateFixedLineList();

                const Count vertexCount = data.LineList.GetItemCount();
                if ( vertexCount == 0 )
                    return;

                // Grow dynamic VB if needed
                if ( vertexCount > data.VertexCapacity )
                {
                    data.VertexCapacity = vertexCount;
                    data.GrowVertexBuffer();
                }

                data.CameraCB.Update( &scene.GetView().GetViewProjectionMatrix() );
                data.Context->FillVertexBuffer(
                    data.DynamicVB, static_cast< void * >( data.LineList.GetData() ), vertexCount );
                data.Context->FillConstantBuffer( data.CameraCB );

                GraphicsState state{};
                state.Framebuffer = res.GetFramebuffer();
                state.Pipeline = data.Pipeline;
                state.VertexBuffers.EmplaceBack( data.DynamicVB, 0u, 0u );
                state.Bindings.PushBack( data.PassBindingSet );

                data.Context->SetGraphicsState( state );
                data.Context->Draw( foundation::NumericCast< Uint32 >( vertexCount ) );
            } );
    }
}
