/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "resource_manager.h"
#include "engine/graphic/renderer_api/renderer_api.h"

namespace smile::graphic
{
    struct RenderSystem::Opaque final
    {
        Scope< RendererAPI > pRendererAPI = nullptr;
        ResourceManager ResManager{};

        DirectX::XMFLOAT4 ClearColor{};

        memory::Ref< Shader > pBoundShader = nullptr;
    };

    RenderSystem::RenderSystem()
    {
        m_pImpl->pRendererAPI = RendererAPI::Create( RendererAPI::API::DirectX11 );
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize( window::Window *pWindow )
    {
        m_pImpl->pRendererAPI->Initialize( pWindow );
        m_pImpl->ResManager.Initialize( m_pImpl->pRendererAPI->GetGraphicsDevice() );
    }

    void RenderSystem::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pImpl->pRendererAPI->GetSwapChain()->Resize( x, y, width, height );
    }

    void RenderSystem::SetClearColor( const DirectX::XMFLOAT4 &color )
    {
        m_pImpl->ClearColor = color;
    }

    void RenderSystem::Clear()
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->Clear( m_pImpl->ClearColor );
    }

    void RenderSystem::BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindVertexBuffer( pVertexBuffer );
    }

    void RenderSystem::UnbindVertexBuffer() const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindVertexBuffer();
    }

    void
    RenderSystem::FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->FillVertexBuffer( pVertexBuffer, pData, vertexCount );
    }

    void RenderSystem::BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindIndexBuffer( pIndexBuffer );
    }

    void RenderSystem::UnbindIndexBuffer() const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindIndexBuffer();
    }

    void RenderSystem::BindShader( memory::Ref< Shader > pShader )
    {
        m_pImpl->pBoundShader = pShader;
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindShader( pShader );
    }

    void RenderSystem::UnbindShader()
    {
        m_pImpl->pBoundShader = nullptr;
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindShader();
    }

    void RenderSystem::BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindFramebuffer( pFramebuffer );
    }

    void RenderSystem::UnbindFramebuffer() const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindFramebuffer();
    }

    void RenderSystem::ClearFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->ClearFramebuffer( pFramebuffer );
    }

    void RenderSystem::BindRasterizerState( memory::Ref< RasterizerState > pRasterizerState ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindRasterizerState( pRasterizerState );
    }

    void RenderSystem::UnbindRasterizerState() const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindRasterizerState();
    }

    void RenderSystem::BindPrimitiveTopology( PrimitiveTopology primitiveTopology ) const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->BindPrimitiveTopology( primitiveTopology );
    }

    void RenderSystem::UnbindPrimitiveTopology() const
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->UnbindPrimitiveTopology();
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->DrawIndexed( indexCount, m_pImpl->pBoundShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->Draw( vertexCount, m_pImpl->pBoundShader );
    }

    void RenderSystem::Present()
    {
        m_pImpl->pRendererAPI->GetSwapChain()->Present();
    }

    ResourceManager &RenderSystem::GetResourceManager()
    {
        return m_pImpl->ResManager;
    }

    RendererAPI *RenderSystem::GetRendererAPI() const
    {
        return m_pImpl->pRendererAPI.get();
    }
}