/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "frame.h"
#include "smile/core/window/window.h"

namespace smile::graphic
{
    RenderSystem::RenderSystem()
    {
        m_API = rhi::RendererBackendType::D3D11;
        m_pDevice = rhi::GraphicsDevice::Create( m_API );
        m_pImmediateCommandList = m_pDevice->CreateCommandList();
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize( const window::Window *pWindow )
    {
        m_ResourceManager.Initialize( m_pDevice.get() );

        m_pSwapChain = m_pDevice->CreateSwapChain( pWindow );

        rhi::Object nativeRenderTarget = m_pSwapChain->GetNativeRenderTarget();

        const rhi::ObjectType objectType = [&]()
        {
            switch ( m_API )
            {
                case rhi::RendererBackendType::D3D11:
                    return rhi::ObjectType::D3D11_Resource;
                default:
                    SM_ASSERT( false );
            }
        }();

        rhi::TextureDescriptor colorDesc;
        colorDesc.Dimension = rhi::TextureDimension::Texture2D;
        colorDesc.TextureFormat = rhi::Format::RGBA8_UNORM;
        colorDesc.Width = pWindow->GetWidth();
        colorDesc.Height = pWindow->GetHeight();
        colorDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };

        Texture::Ref pColorTexture =
            m_ResourceManager.CreateTextureFromNative( nativeRenderTarget, objectType, colorDesc );

        FramebufferAttachment depthAttachment =
            m_ResourceManager.CreateDepthAttachment( pWindow->GetWidth(), pWindow->GetHeight() );

        m_pBackBuffer = m_ResourceManager.CreateFramebuffer(
            { FramebufferAttachment{ pColorTexture, colorDesc.TextureFormat, false } }, depthAttachment );
    }

    void RenderSystem::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pSwapChain->Resize( x, y, width, height );
    }

    void RenderSystem::Clear( Framebuffer::Ref pFramebuffer,
        const std::optional< math::Color > &color,
        std::optional< float > depth,
        std::optional< Uint8 > stencil )
    {
        if ( color.has_value() )
        {
            for ( const auto &colorAttachment : pFramebuffer->GetColorAttachments() )
            {
                m_pImmediateCommandList->ClearTexture( colorAttachment.pTexture->GetHandle(),
                    rhi::TextureSubresourceSet{},
                    color.value_or( math::Color{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
            }
        }

        if ( depth.has_value() || stencil.has_value() )
        {
            const auto &depthAttachment = pFramebuffer->GetDepthAttachment();
            m_pImmediateCommandList->ClearDepthStencilTexture(
                depthAttachment.pTexture->GetHandle(), rhi::TextureSubresourceSet{}, depth, stencil );
        }
    }

    void RenderSystem::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
        m_pImmediateCommandList->Open();
    }

    void RenderSystem::EndFrame()
    {
        Present();
        m_pImmediateCommandList->Close();
        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }

    void RenderSystem::SetGraphicsState( const GraphicsState &state )
    {
        rhi::GraphicsState graphicsState{};
        graphicsState.Pipeline = state.pPipeline->GetHandle();
        graphicsState.Framebuffer = state.pFramebuffer->GetHandle();
        graphicsState.Viewport.Viewports.PushBack( state.pFramebuffer->GetViewport( 0.0f, 1.0f ) );

        for ( const auto &pBinding : state.pBindings )
        {
            graphicsState.Bindings.PushBack( pBinding->GetHandle() );
        }

        for ( const auto &vertexBuffer : state.VertexBuffers )
        {
            rhi::VertexBufferBinding vertexBufferBinding{
                vertexBuffer.pVertexBuffer->GetHandle(), vertexBuffer.Slot, vertexBuffer.Offset };

            graphicsState.VertexBuffers.PushBack( std::move( vertexBufferBinding ) );
        }

        if ( state.IndexBuffer.pIndexBuffer )
        {
            graphicsState.IndexBuffer = rhi::IndexBufferBinding{
                state.IndexBuffer.pIndexBuffer->GetHandle(), state.IndexBuffer.BufferFormat, state.IndexBuffer.Offset };
        }

        m_pImmediateCommandList->SetGraphicsState( graphicsState );
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        rhi::DrawIndexedParams params{ indexCount, 0, 0 };
        m_pImmediateCommandList->DrawIndexed( params );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        rhi::DrawParams params{ vertexCount, 0 };
        m_pImmediateCommandList->Draw( params );
    }

    void RenderSystem::Present()
    {
        m_pSwapChain->Present();
    }

    void RenderSystem::FillVertexBuffer( VertexBuffer::Ref pVertexBuffer, void *pData, const Count vertexCount ) const
    {
        m_pImmediateCommandList->FillBuffer(
            pVertexBuffer->GetHandle(), pData, vertexCount * pVertexBuffer->GetBufferLayout().GetStride() );
    }

    void RenderSystem::FillConstantBuffer( ConstantBuffer::Ref constantBuffer ) const
    {
        m_pImmediateCommandList->FillBuffer(
            constantBuffer->GetHandle(), constantBuffer->GetBuffer(), constantBuffer->GetDescriptor().GetSize() );
    }
}