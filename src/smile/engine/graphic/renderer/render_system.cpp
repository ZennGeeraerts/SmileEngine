/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "engine/graphic/renderer_backend/renderer_backend.h"

namespace smile::graphic
{
    RenderSystem::RenderSystem()
    {
        m_pRendererAPI = RendererBackend::Create( RendererBackend::API::DirectX11 );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize( const window::Window *pWindow )
    {
        m_ResourceManager.Initialize( m_pRendererAPI->GetGraphicsDevice() );

        m_pSwapChain = m_pRendererAPI->GetGraphicsDevice()->CreateSwapChain( pWindow );
    }

    void RenderSystem::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pRendererAPI->GetGraphicsDevice()->ResizeBackBuffer( m_pSwapChain, x, y, width, height );
    }

    void RenderSystem::Clear()
    {
        if ( m_pBoundFramebuffer )
        {
            m_pRendererAPI->GetGraphicsContext()->ClearFramebuffer( m_pBoundFramebuffer );
        }
        else
        {
            m_pRendererAPI->GetGraphicsContext()->ClearBackBuffer( m_pSwapChain, m_ClearColor );
        }
    }

    void RenderSystem::BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const
    {
        m_pRendererAPI->GetGraphicsContext()->BindVertexBuffer( pVertexBuffer );
    }

    void RenderSystem::UnbindVertexBuffer() const
    {
        m_pRendererAPI->GetGraphicsContext()->UnbindVertexBuffer();
    }

    void
    RenderSystem::FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const
    {
        m_pRendererAPI->GetGraphicsContext()->FillVertexBuffer( pVertexBuffer, pData, vertexCount );
    }

    void RenderSystem::BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const
    {
        m_pRendererAPI->GetGraphicsContext()->BindIndexBuffer( pIndexBuffer );
    }

    void RenderSystem::UnbindIndexBuffer() const
    {
        m_pRendererAPI->GetGraphicsContext()->UnbindIndexBuffer();
    }

    void RenderSystem::BindShader( memory::Ref< Shader > pShader )
    {
        m_pRendererAPI->GetGraphicsContext()->BindShader( pShader );
        m_pBoundShader = pShader;
    }

    void RenderSystem::UnbindShader()
    {
        m_pRendererAPI->GetGraphicsContext()->UnbindShader();
        m_pBoundShader = nullptr;
    }

    void RenderSystem::BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        m_pRendererAPI->GetGraphicsContext()->BindFramebuffer( pFramebuffer );
        m_pBoundFramebuffer = pFramebuffer;
    }

    void RenderSystem::BindBackBuffer()
    {
        m_pRendererAPI->GetGraphicsContext()->BindBackBuffer( m_pSwapChain );
        m_pBoundFramebuffer = nullptr;
    }

    void RenderSystem::BindRasterizerState( memory::Ref< RasterizerState > pRasterizerState ) const
    {
        m_pRendererAPI->GetGraphicsContext()->BindRasterizerState( pRasterizerState );
    }

    void RenderSystem::UnbindRasterizerState() const
    {
        m_pRendererAPI->GetGraphicsContext()->UnbindRasterizerState();
    }

    void RenderSystem::BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const
    {
        m_pRendererAPI->GetGraphicsContext()->BindPrimitiveTopology( primitiveTopology );
    }

    void RenderSystem::UnbindPrimitiveTopology() const
    {
        m_pRendererAPI->GetGraphicsContext()->UnbindPrimitiveTopology();
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        m_pRendererAPI->GetGraphicsContext()->DrawIndexed( indexCount, m_pBoundShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        m_pRendererAPI->GetGraphicsContext()->Draw( vertexCount, m_pBoundShader );
    }

    void RenderSystem::Present()
    {
        m_pSwapChain->Present();
    }

    RendererBackend *RenderSystem::GetRendererAPI() const
    {
        return m_pRendererAPI.get();
    }
}