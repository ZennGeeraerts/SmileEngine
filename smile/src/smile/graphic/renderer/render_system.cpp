/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "smile/graphic/renderer_backend/renderer_backend.h"

namespace smile::graphic
{
    RenderSystem::RenderSystem()
    {
        m_pRendererBackend = CreateScope< RendererBackend >( RendererBackendType::DirectX11 );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize( const window::Window *pWindow )
    {
        m_ResourceManager.Initialize( m_pRendererBackend->GetGraphicsDevice() );

        m_pSwapChain = m_pRendererBackend->GetGraphicsDevice()->CreateSwapChain( pWindow );
    }

    void RenderSystem::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pRendererBackend->GetGraphicsDevice()->ResizeBackBuffer( m_pSwapChain, x, y, width, height );
    }

    void RenderSystem::Clear()
    {
        if ( m_pBoundFramebuffer )
        {
            m_pRendererBackend->GetGraphicsContext()->ClearFramebuffer( m_pBoundFramebuffer );
        }
        else
        {
            m_pRendererBackend->GetGraphicsContext()->ClearBackBuffer( m_pSwapChain, m_ClearColor );
        }
    }

    void RenderSystem::BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const
    {
        m_pRendererBackend->GetGraphicsContext()->BindVertexBuffer(
            pVertexBuffer->Handle, pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::UnbindVertexBuffer() const
    {
        m_pRendererBackend->GetGraphicsContext()->UnbindVertexBuffer();
    }

    void
    RenderSystem::FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const
    {
        m_pRendererBackend->GetGraphicsContext()->FillBuffer(
            pVertexBuffer->Handle, pData, vertexCount * pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const
    {
        m_pRendererBackend->GetGraphicsContext()->BindIndexBuffer( pIndexBuffer->Handle );
    }

    void RenderSystem::UnbindIndexBuffer() const
    {
        m_pRendererBackend->GetGraphicsContext()->UnbindIndexBuffer();
    }

    void RenderSystem::BindUniformBuffer( const memory::Ref< UniformBuffer > &pUniformBuffer ) const
    {
    }

    void RenderSystem::UnbindUniformBuffer() const
    {
    }

    void RenderSystem::BindShader( memory::Ref< Shader > pShader )
    {
        m_pRendererBackend->GetGraphicsContext()->BindShader( pShader );
        m_pBoundShader = pShader;
    }

    void RenderSystem::UnbindShader()
    {
        m_pRendererBackend->GetGraphicsContext()->UnbindShader();
        m_pBoundShader = nullptr;
    }

    void RenderSystem::BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        m_pRendererBackend->GetGraphicsContext()->BindFramebuffer( pFramebuffer );
        m_pBoundFramebuffer = pFramebuffer;
    }

    void RenderSystem::BindBackBuffer()
    {
        m_pRendererBackend->GetGraphicsContext()->BindBackBuffer( m_pSwapChain );
        m_pBoundFramebuffer = nullptr;
    }

    void RenderSystem::BindRasterizerState( memory::Ref< RasterizerState > pRasterizerState ) const
    {
        m_pRendererBackend->GetGraphicsContext()->BindRasterizerState( pRasterizerState );
    }

    void RenderSystem::UnbindRasterizerState() const
    {
        m_pRendererBackend->GetGraphicsContext()->UnbindRasterizerState();
    }

    void RenderSystem::BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const
    {
        m_pRendererBackend->GetGraphicsContext()->BindPrimitiveTopology( primitiveTopology );
    }

    void RenderSystem::UnbindPrimitiveTopology() const
    {
        m_pRendererBackend->GetGraphicsContext()->UnbindPrimitiveTopology();
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        m_pRendererBackend->GetGraphicsContext()->DrawIndexed( indexCount, m_pBoundShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        m_pRendererBackend->GetGraphicsContext()->Draw( vertexCount, m_pBoundShader );
    }

    void RenderSystem::Present()
    {
        m_pSwapChain->Present();
    }

    RendererBackend *RenderSystem::GetRendererAPI() const
    {
        return m_pRendererBackend.get();
    }
}