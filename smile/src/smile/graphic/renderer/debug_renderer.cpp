/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "debug_renderer.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource_manager.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    void DebugRenderer::Initialize()
    {
        auto &shaderLibrary = RenderEngine::GetShaderLibrary();
        BufferLayout bufferLayout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float4, "COLOR" } };
        m_pShader = shaderLibrary.Load( "resources/shaders/DebugRenderer.fx", bufferLayout );

        CreateVertexBuffer();
    }

    void DebugRenderer::ShutDown()
    {
        m_LineList.clear();
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
            m_LineList.emplace_back( VertexPosCol{ vertStart, gridColor } );
            vertStart.x += size;
            m_LineList.emplace_back( VertexPosCol{ vertStart, gridColor } );

            // Horizontal
            vertStart = DirectX::XMFLOAT3( lineOffset, 0, startOffset );
            m_LineList.emplace_back( VertexPosCol{ vertStart, gridColor } );
            vertStart.z += size;
            m_LineList.emplace_back( VertexPosCol{ vertStart, gridColor } );
        }

        // Axis
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed ) } );
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 30, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkRed ) } );
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen ) } );
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 0, 30, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkGreen ) } );
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 0, 0, 0 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue ) } );
        m_LineList.emplace_back( VertexPosCol{
            DirectX::XMFLOAT3( 0, 0, 30 ), static_cast< DirectX::XMFLOAT4 >( DirectX::Colors::DarkBlue ) } );
    }

    void DebugRenderer::CreateVertexBuffer()
    {
        GPUBufferDescriptor descriptor{};
        descriptor.Usage = BufferUsage::Dynamic;
        descriptor.Size = m_VertexCount * sizeof( VertexPosCol );
        descriptor.CPUAccess = BufferCPUAccess::Write;

        m_pVertexBuffer = RenderEngine::GetRenderSystem().GetResourceManager().CreateVertexBuffer(
            descriptor, sizeof( VertexPosCol ) );
    }

    void DebugRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &m_ViewProjectionMatrix, viewProjectionMatrixMat );
    }

    void DebugRenderer::OnRender()
    {
        const Uint32 vertexCount = m_LineList.size();

        if ( vertexCount <= 0 )
            return;

        if ( vertexCount > m_VertexCount )
        {
            m_VertexCount = vertexCount;
            CreateVertexBuffer();
        }

        RenderSystem &renderSystem = RenderEngine::GetRenderSystem();

        renderSystem.FillVertexBuffer( m_pVertexBuffer, m_LineList.data(), vertexCount );

        renderSystem.BindPrimitiveTopology( PrimitiveTopology::LineList );

        renderSystem.BindVertexBuffer( m_pVertexBuffer );
        renderSystem.BindShader( m_pShader );

        m_pShader->UploadMat4( "ViewProjection", m_ViewProjectionMatrix );

        DirectX::XMFLOAT4X4 worldMatrix{};
        DirectX::XMStoreFloat4x4( &worldMatrix, DirectX::XMMatrixIdentity() );
        m_pShader->UploadMat4( "World", worldMatrix );

        renderSystem.Draw( vertexCount );

        renderSystem.UnbindPrimitiveTopology();
    }

    void DebugRenderer::EndScene()
    {
        m_LineList.clear();
    }

    void DebugRenderer::SubmitLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &color )
    {
        m_LineList.emplace_back( VertexPosCol{ start, color } );
        m_LineList.emplace_back( VertexPosCol{ end, color } );
    }

    void DebugRenderer::SubmitLine( const DirectX::XMFLOAT3 &start,
        const DirectX::XMFLOAT3 &end,
        const DirectX::XMFLOAT4 &colorStart,
        const DirectX::XMFLOAT4 &colorEnd )
    {
        m_LineList.emplace_back( VertexPosCol{ start, colorStart } );
        m_LineList.emplace_back( VertexPosCol{ end, colorEnd } );
    }
}