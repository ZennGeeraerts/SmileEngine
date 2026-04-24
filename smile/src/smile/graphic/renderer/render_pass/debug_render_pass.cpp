/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_render_pass.h"

#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/shader/shader_library.h"
#include "smile/graphic/shader/shader_asset.h"
#include "smile/graphic/renderer/shader/program.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    DebugRenderPass::DebugRenderPass( RenderContext &context,
        ResourceManager &resourceManager,
        const ShaderLibrary &shaderLib ) noexcept
        : m_Context{ context },
          m_ResourceManager{ resourceManager },
          m_ShaderLib{ shaderLib },
          m_VertexLayout{ { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RGBA32_FLOAT, "COLOR" } }
    {
    }

    void DebugRenderPass::Initialize()
    {
        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );

            m_CameraCB = m_ResourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            auto vertexShaderAsset = m_ShaderLib.GetShader( "debug_renderer.vs" );
            auto pixelShaderAsset = m_ShaderLib.GetShader( "pos_col.ps" );

            auto program = Program::Create( vertexShaderAsset, pixelShaderAsset );

            GraphicsPipelineDescriptor psoDesc{};
            psoDesc.Topology = rhi::PrimitiveTopology::LineList;
            psoDesc.InputLayout = m_VertexLayout;
            psoDesc.VertexShader = m_ResourceManager.CreateVertexShader( vertexShaderAsset );
            psoDesc.PixelShader = m_ResourceManager.CreatePixelShader( pixelShaderAsset );

            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, m_CameraCB.GetHandle() ) } };

            BindingLayout bindingLayout;
            m_ResourceManager.CreateBindingSetAndLayout(
                bindingSetDesc, { rhi::ShaderStage::Vertex }, bindingLayout, m_BindingSet );

            psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );

            psoDesc.RenderState.RasterizerState.CullMode = rhi::CullMode::None;

            m_Pipeline = m_ResourceManager.CreateGraphicsPipeline( psoDesc );
        }

        CreateVertexBuffer();
    }

    void DebugRenderPass::ShutDown()
    {
        m_LineList.Clear();
    }

    void DebugRenderPass::CreateFixedLineList()
    {
        // Grid
        const Uint32 numGridLines = 20;
        const float gridSpacing = 1.0f;

        const float startOffset = -( static_cast< int >( numGridLines ) / 2 ) * gridSpacing;
        const float size = ( numGridLines - 1 ) * gridSpacing;
        const auto gridColor = static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::LightGray );
        for ( Uint32 i = 0; i < numGridLines; ++i )
        {
            // Vertical
            const float lineOffset = startOffset + gridSpacing * i;
            auto vertStart = DirectX::XMFLOAT3( startOffset, 0, lineOffset );
            m_LineList.EmplaceBack( vertStart, gridColor );
            vertStart.x += size;
            m_LineList.EmplaceBack( vertStart, gridColor );

            // Horizontal
            vertStart = DirectX::XMFLOAT3( lineOffset, 0, startOffset );
            m_LineList.PushBack( VertexPosCol{ vertStart, gridColor } );
            vertStart.z += size;
            m_LineList.PushBack( VertexPosCol{ vertStart, gridColor } );
        }

        // Axis
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed ) );
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 30, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed ) );
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen ) );
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 0, 30, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen ) );
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue ) );
        m_LineList.EmplaceBack(
            DirectX::XMFLOAT3( 0, 0, 30 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue ) );
    }

    void DebugRenderPass::CreateVertexBuffer()
    {
        m_VertexBuffer = m_ResourceManager.CreateDynamicVertexBuffer( m_VertexCount, m_VertexLayout );
    }

    void DebugRenderPass::BeginPass( const View &view )
    {
        m_CameraCB.Update( &view.GetViewProjectionMatrix() );

        CreateFixedLineList();
    }

    void DebugRenderPass::Execute( const Framebuffer &framebuffer )
    {
        const Count vertexCount = m_LineList.GetItemCount();

        if ( vertexCount <= 0 )
            return;

        if ( vertexCount > m_VertexCount )
        {
            m_VertexCount = vertexCount;
            CreateVertexBuffer();
        }

        m_Context.FillVertexBuffer( m_VertexBuffer, m_LineList.GetData(), vertexCount );
        m_Context.FillConstantBuffer( m_CameraCB );

        GraphicsState state{};
        state.Framebuffer = framebuffer;
        state.Pipeline = m_Pipeline;
        state.VertexBuffers.EmplaceBack( m_VertexBuffer, 0u, 0u );
        state.Bindings.PushBack( m_BindingSet );

        m_Context.SetGraphicsState( state );
        m_Context.Draw( vertexCount );
    }

    void DebugRenderPass::EndPass()
    {
        m_LineList.Clear();
    }

    void DebugRenderPass::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &color )
    {
        m_LineList.EmplaceBack( start, color );
        m_LineList.EmplaceBack( end, color );
    }

    void DebugRenderPass::DrawLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &colorStart,
        const DirectX::XMFLOAT4 &colorEnd )
    {
        m_LineList.EmplaceBack( start, colorStart );
        m_LineList.EmplaceBack( end, colorEnd );
    }
}