/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_renderer.h"

#include "render_engine.h"
#include "resource/resource_manager.h"
#include "smile/graphic/shader/shader_library.h"
#include "smile/graphic/shader/shader_asset.h"
#include "shader/program.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    void DebugRenderer::Initialize()
    {
        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();
        const auto &shaderLibrary = RenderEngine::GetShaderLibrary();

        rhi::BufferLayout vertexLayout{
            { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RGBA32_FLOAT, "COLOR" } };

        {
            auto vertexShaderAsset = shaderLibrary.GetShader( "debug_renderer.vs" );
            auto pixelShaderAsset = shaderLibrary.GetShader( "pos_col.ps" );

            auto program = Program::Create( vertexShaderAsset, pixelShaderAsset );

            GraphicsPipelineDescriptor psoDesc{};
            psoDesc.Topology = rhi::PrimitiveTopology::LineList;
            psoDesc.InputLayout = vertexLayout;
            psoDesc.pVertexShader = resourceManager.CreateVertexShader( vertexShaderAsset );
            psoDesc.pPixelShader = resourceManager.CreatePixelShader( pixelShaderAsset );

            auto bindingLayout = rhi::BindingLayout{ { rhi::ShaderStage::Vertex } };
            bindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
            psoDesc.BindingLayouts.PushBack( std::move( bindingLayout ) );

            psoDesc.RenderState.RasterizerState.CullMode = rhi::CullMode::None;

            m_pPipeline = resourceManager.CreateGraphicsPipeline( psoDesc );
        }

        {
            ConstantBufferDescriptor cameraCBDesc{};
            cameraCBDesc.Add( "ViewProjection", ConstantType::Mat4 );

            m_pCameraCB = resourceManager.CreateConstantBuffer( cameraCBDesc );
        }

        {
            rhi::BindingSetDescriptor bindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, m_pCameraCB->GetHandle() ) } };

            m_pBindingSet = resourceManager.CreateBindingSet( bindingSetDesc, { rhi::ShaderStage::Vertex } );
        }

        CreateVertexBuffer();
    }

    void DebugRenderer::ShutDown()
    {
        m_LineList.Clear();
    }

    void DebugRenderer::CreateFixedLineList()
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

    void DebugRenderer::CreateVertexBuffer()
    {
        const auto &vertexLayout = m_pPipeline->GetDescriptor().InputLayout;

        m_pVertexBuffer =
            RenderEngine::GetRenderSystem().GetResourceManager().CreateDynamicVertexBuffer( m_VertexCount, vertexLayout );
    }

    void DebugRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = DirectX::XMMatrixTranspose( viewMatrixMat * projectionMatrixMat );

        DirectX::XMFLOAT4X4 viewProjectionMatrix;
        DirectX::XMStoreFloat4x4( &m_ViewProjectionMatrix, viewProjectionMatrixMat );

        m_pCameraCB->Update( &m_ViewProjectionMatrix );

        CreateFixedLineList();
    }

    void DebugRenderer::OnRender( Framebuffer::Ref framebuffer )
    {
        const Count vertexCount = m_LineList.GetItemCount();

        if ( vertexCount <= 0 )
            return;

        if ( vertexCount > m_VertexCount )
        {
            m_VertexCount = vertexCount;
            CreateVertexBuffer();
        }

        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.FillVertexBuffer( m_pVertexBuffer, m_LineList.GetData(), vertexCount );
        renderSystem.FillConstantBuffer( m_pCameraCB );

        GraphicsState state{};
        state.pFramebuffer = framebuffer;
        state.pPipeline = m_pPipeline;
        state.VertexBuffers.EmplaceBack( m_pVertexBuffer, 0u, 0u );
        state.pBindings.PushBack( m_pBindingSet );

        renderSystem.SetGraphicsState( state );
        renderSystem.Draw( vertexCount );
    }

    void DebugRenderer::EndScene()
    {
        m_LineList.Clear();
    }

    void DebugRenderer::SubmitLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &color )
    {
        m_LineList.EmplaceBack( start, color );
        m_LineList.EmplaceBack( end, color );
    }

    void DebugRenderer::SubmitLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &colorStart,
        const DirectX::XMFLOAT4 &colorEnd )
    {
        m_LineList.EmplaceBack( start, colorStart );
        m_LineList.EmplaceBack( end, colorEnd );
    }
}