/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "resource_manager.h"
#include "engine/graphic/renderer_api/renderer_api.h"
#include "engine/graphic/renderer_api/shader/shader.h"

namespace smile::graphic
{
    struct RenderSystem::Opaque final
    {
        Scope< RendererAPI > pRendererAPI = nullptr;
        ResourceManager ResManager{};
        DirectX::XMFLOAT4 ClearColor{};
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

    void RenderSystem::DrawIndexed( Uint32 indexCount, const memory::Ref< Shader > &pShader )
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->DrawIndexed( indexCount, pShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount, const memory::Ref< Shader > &pShader )
    {
        m_pImpl->pRendererAPI->GetGraphicsContext()->Draw( vertexCount, pShader );
    }

    void RenderSystem::Present()
    {
        m_pImpl->pRendererAPI->GetSwapChain()->Present();
    }

    GraphicsContext* RenderSystem::GetGraphicsContext() const
    {
        return m_pImpl->pRendererAPI->GetGraphicsContext();
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