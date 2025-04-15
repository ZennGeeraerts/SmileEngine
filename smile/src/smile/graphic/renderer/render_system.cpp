/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

namespace smile::graphic
{
    RenderSystem::RenderSystem()
    {
        m_API = RendererBackendType::DirectX11;
        m_pDevice = GraphicsDevice::Create( m_API );
        m_pGraphicsContext = m_pDevice->CreateGraphicsContext();
    }

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::Initialize( const window::Window *pWindow )
    {
        m_ResourceManager.Initialize( m_pDevice.get() );

        m_pSwapChain = m_pDevice->CreateSwapChain( pWindow );
    }

    void RenderSystem::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        m_pSwapChain->Resize( x, y, width, height );
    }

    void RenderSystem::Clear()
    {
        if ( m_pBoundFramebuffer )
        {
            m_pGraphicsContext->ClearFramebuffer( m_pBoundFramebuffer->Handle );
        }
        else
        {
            m_pGraphicsContext->ClearBackBuffer( m_pSwapChain, m_ClearColor );
        }
    }

    void RenderSystem::BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const
    {
        m_pGraphicsContext->BindVertexBuffer( pVertexBuffer->Handle, pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::UnbindVertexBuffer() const
    {
        m_pGraphicsContext->UnbindVertexBuffer();
    }

    void
    RenderSystem::FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const
    {
        m_pGraphicsContext->FillBuffer( pVertexBuffer->Handle, pData, vertexCount * pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const
    {
        m_pGraphicsContext->BindIndexBuffer( pIndexBuffer->Handle );
    }

    void RenderSystem::UnbindIndexBuffer() const
    {
        m_pGraphicsContext->UnbindIndexBuffer();
    }

    void RenderSystem::BindUniformBuffer( const memory::Ref< UniformBuffer > &pUniformBuffer ) const
    {
    }

    void RenderSystem::UnbindUniformBuffer() const
    {
    }

    void RenderSystem::BindShader( memory::Ref< Shader > pShader )
    {
        m_pGraphicsContext->BindShader( pShader );
        m_pBoundShader = pShader;
    }

    void RenderSystem::UnbindShader()
    {
        m_pGraphicsContext->UnbindShader();
        m_pBoundShader = nullptr;
    }

    void RenderSystem::BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        m_pGraphicsContext->BindFramebuffer( pFramebuffer->Handle );
        m_pBoundFramebuffer = pFramebuffer;
    }

    void RenderSystem::BindBackBuffer()
    {
        m_pGraphicsContext->BindBackBuffer( m_pSwapChain );
        m_pBoundFramebuffer = nullptr;
    }

    void RenderSystem::SetState( const RenderState &state ) const
    {
        m_pGraphicsContext->SetState( state );
    }

    void *RenderSystem::ReadTexture( memory::Ref< Texture > pTexture ) const
    {
        return m_pGraphicsContext->ReadTexture( pTexture->Handle );
    }

    void *RenderSystem::ReadTexture( memory::Ref< Framebuffer > pFramebuffer, Uint32 index ) const
    {
        return m_pGraphicsContext->ReadTexture( pFramebuffer->Handle, index );
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        m_pGraphicsContext->DrawIndexed( indexCount, m_pBoundShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        m_pGraphicsContext->Draw( vertexCount, m_pBoundShader );
    }

    void RenderSystem::Present()
    {
        m_pSwapChain->Present();
    }
}