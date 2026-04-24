/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_context.h"

#include "frame.h"
#include "smile/core/window/window.h"

#include "resource/resource_manager.h"

namespace smile::graphic
{
    RenderContext::RenderContext() = default;
    RenderContext::~RenderContext() = default;

    void RenderContext::Initialize( rhi::RendererBackendType api, ResourceManager *resourceManager )
    {
        m_pDevice = rhi::GraphicsDevice::Create( api );
        m_pImmediateCommandList = m_pDevice->CreateCommandList();
        m_ResourceManager = resourceManager;
    }

    void RenderContext::Clear( const Framebuffer &framebuffer,
        const std::optional< math::Color > &color,
        std::optional< float > depth,
        std::optional< Uint8 > stencil )
    {
        const auto &attachmentSet = m_ResourceManager->GetFramebufferAttachmentSet( framebuffer );

        if ( color.has_value() )
        {
            for ( const auto &colorAttachment : attachmentSet.ColorAttachments )
            {
                m_pImmediateCommandList->ClearTexture( colorAttachment.Texture.GetHandle(),
                    rhi::TextureSubresourceSet{},
                    color.value_or( math::Color{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
            }
        }

        if ( depth.has_value() || stencil.has_value() )
        {
            m_pImmediateCommandList->ClearDepthStencilTexture(
                attachmentSet.DepthAttachment.Texture.GetHandle(), rhi::TextureSubresourceSet{}, depth, stencil );
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
        graphicsState.Pipeline = state.Pipeline.GetHandle();
        graphicsState.Framebuffer = state.Framebuffer.GetHandle();
        graphicsState.Viewport.Viewports.PushBack( state.Framebuffer.GetViewport( 0.0f, 1.0f ) );

        for ( const auto &binding : state.Bindings )
        {
            graphicsState.Bindings.PushBack( binding.GetHandle() );
        }

        for ( const auto &vertexBuffer : state.VertexBuffers )
        {
            rhi::VertexBufferBinding vertexBufferBinding{
                vertexBuffer.VertexBuffer.GetHandle(), vertexBuffer.Slot, vertexBuffer.Offset };

            graphicsState.VertexBuffers.PushBack( std::move( vertexBufferBinding ) );
        }

        if ( state.IndexBuffer.IndexBuffer.IsValid() )
        {
            graphicsState.IndexBuffer = rhi::IndexBufferBinding{
                state.IndexBuffer.IndexBuffer.GetHandle(), state.IndexBuffer.BufferFormat, state.IndexBuffer.Offset };
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

    void RenderContext::FillVertexBuffer( const VertexBuffer &vertexBuffer, void *pData, const Count vertexCount ) const
    {
        m_pImmediateCommandList->FillBuffer( vertexBuffer.GetHandle(), pData, vertexCount * vertexBuffer.GetStride() );
    }

    void RenderContext::FillConstantBuffer( const ConstantBuffer &constantBuffer ) const
    {
        m_pImmediateCommandList->FillBuffer(
            constantBuffer.GetHandle(), constantBuffer.GetBuffer(), constantBuffer.GetSize() );
    }
}