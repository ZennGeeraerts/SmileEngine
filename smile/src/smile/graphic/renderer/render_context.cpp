/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_context.h"

#include "frame.h"
#include "smile/core/window/window.h"

namespace smile::graphic
{
    RenderContext::RenderContext() = default;
    RenderContext::~RenderContext() = default;

    void RenderContext::Initialize( rhi::RendererBackendType api )
    {
        m_pDevice = rhi::GraphicsDevice::Create( api );
        m_pImmediateCommandList = m_pDevice->CreateCommandList();

        m_ResourceManager.Initialize( m_pDevice.get() );
    }

    void RenderContext::Clear( Framebuffer::Ref pFramebuffer,
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

    void RenderContext::Open()
    {
        m_pImmediateCommandList->Open();
    }

    void RenderContext::Close()
    {
        m_pImmediateCommandList->Close();
    }

    void RenderContext::SetGraphicsState( const GraphicsState &state )
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

    void RenderContext::DrawIndexed( Uint32 indexCount )
    {
        rhi::DrawIndexedParams params{ indexCount, 0, 0 };
        m_pImmediateCommandList->DrawIndexed( params );
    }

    void RenderContext::Draw( Uint32 vertexCount )
    {
        rhi::DrawParams params{ vertexCount, 0 };
        m_pImmediateCommandList->Draw( params );
    }

    void RenderContext::FillVertexBuffer( VertexBuffer::Ref pVertexBuffer, void *pData, const Count vertexCount ) const
    {
        m_pImmediateCommandList->FillBuffer(
            pVertexBuffer->GetHandle(), pData, vertexCount * pVertexBuffer->GetBufferLayout().GetStride() );
    }

    void RenderContext::FillConstantBuffer( ConstantBuffer::Ref constantBuffer ) const
    {
        m_pImmediateCommandList->FillBuffer(
            constantBuffer->GetHandle(), constantBuffer->GetBuffer(), constantBuffer->GetDescriptor().GetSize() );
    }
}