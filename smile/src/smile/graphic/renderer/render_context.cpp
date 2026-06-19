/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_context.h"

#include "frame.h"
#include "smile/core/window/window.h"

#include "resource/vertex_buffer.h"
#include "shader/constant_buffer.h"

namespace smile::graphic
{
    RenderContext::RenderContext( rhi::CommandList &immediateCommandList ) noexcept
        : m_ImmediateCommandList{ immediateCommandList }
    {
    }

    RenderContext::~RenderContext() = default;

    void RenderContext::Clear( const FramebufferAttachmentSet &attachmentSet,
        const std::optional< math::Color > &color,
        std::optional< float > depth,
        std::optional< Uint8 > stencil )
    {
        if ( color.has_value() )
        {
            for ( const auto &colorAttachment : attachmentSet.ColorAttachments )
            {
                m_ImmediateCommandList.ClearTexture( colorAttachment.Texture.GetHandle(),
                    rhi::TextureSubresourceSet{},
                    color.value_or( math::Color{ 0.0f, 0.0f, 0.0f, 1.0f } ) );
            }
        }

        if ( depth.has_value() || stencil.has_value() )
        {
            m_ImmediateCommandList.ClearDepthStencilTexture(
                attachmentSet.DepthAttachment.Texture.GetHandle(), rhi::TextureSubresourceSet{}, depth, stencil );
        }
    }

    void RenderContext::Open()
    {
        m_ImmediateCommandList.Open();
    }

    void RenderContext::Close()
    {
        m_ImmediateCommandList.Close();
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

        m_ImmediateCommandList.SetGraphicsState( graphicsState );
    }

    void RenderContext::DrawIndexed( const Count indexCount )
    {
        rhi::DrawIndexedParams params{ indexCount, 0, 0 };
        m_ImmediateCommandList.DrawIndexed( params );
    }

    void RenderContext::Draw( const Count vertexCount )
    {
        rhi::DrawParams params{ vertexCount, 0 };
        m_ImmediateCommandList.Draw( params );
    }

    void RenderContext::DrawInstanced( const Count vertexCount, const Count instanceCount )
    {
        rhi::DrawInstancedParams params{ vertexCount, instanceCount, 0, 0 };
        m_ImmediateCommandList.DrawInstanced( params );
    }

    void RenderContext::DrawInstancedIndexed( const Count indexCount, const Count instanceCount )
    {
        rhi::DrawIndexedInstancedParams params{ indexCount, instanceCount, 0, 0, 0 };
        m_ImmediateCommandList.DrawIndexedInstanced( params );
    }

    void RenderContext::FillVertexBuffer( const VertexBuffer &vertexBuffer, void *pData, const Count vertexCount ) const
    {
        m_ImmediateCommandList.FillBuffer( vertexBuffer.GetHandle(), pData, vertexCount * vertexBuffer.GetStride() );
    }

    void RenderContext::FillConstantBuffer( const ConstantBuffer &constantBuffer ) const
    {
        m_ImmediateCommandList.FillBuffer(
            constantBuffer.GetHandle(), constantBuffer.GetBuffer(), constantBuffer.GetSize() );
    }

    void RenderContext::CopyTexture( const Texture &dst,
        const rhi::TextureSlice &dstSlice,
        const Texture &src,
        const rhi::TextureSlice &srcSlice ) const
    {
        m_ImmediateCommandList.CopyTexture( dst.GetHandle(), dstSlice, src.GetHandle(), srcSlice );
    }
}