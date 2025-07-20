/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_system.h"

#include "frame.h"

namespace smile::graphic
{
    RenderSystem::RenderSystem()
    {
        m_API = RendererBackendType::DirectX11;
        m_pDevice = GraphicsDevice::Create( m_API );
        m_pImmediateCommandList = m_pDevice->CreateCommandList();
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
            for ( const auto &colorAttachment : m_pBoundFramebuffer->GetColorAttachments() )
            {
                m_pImmediateCommandList->ClearTexture(
                    colorAttachment.pTexture->GetHandle(), rhi::TextureSubresourceSet{}, m_ClearColor );
            }

            const auto &depthAttachment = m_pBoundFramebuffer->GetDepthAttachment();
            m_pImmediateCommandList->ClearDepthStencilTexture(
                depthAttachment.pTexture->GetHandle(), rhi::TextureSubresourceSet{}, true, 1.0f, true, 0 );
        }
        else
        {
            m_pImmediateCommandList->ClearBackBuffer( m_pSwapChain, m_ClearColor );
        }
    }

    void RenderSystem::BeginFrame()
    {
        m_CurrentFrameIndex = AssignFrameData();
    }

    void RenderSystem::EndFrame()
    {
        Present();
        ReleaseFrameData( m_RenderedFrameIndex );
        m_RenderedFrameIndex = m_CurrentFrameIndex;
    }

    void RenderSystem::BindVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer ) const
    {
        m_pImmediateCommandList->BindVertexBuffer( pVertexBuffer->Handle, pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::UnbindVertexBuffer() const
    {
        m_pImmediateCommandList->UnbindVertexBuffer();
    }

    void
    RenderSystem::FillVertexBuffer( memory::Ref< VertexBuffer > pVertexBuffer, void *pData, Uint32 vertexCount ) const
    {
        m_pImmediateCommandList->FillBuffer( pVertexBuffer->Handle, pData, vertexCount * pVertexBuffer->Layout.GetStride() );
    }

    void RenderSystem::BindIndexBuffer( memory::Ref< IndexBuffer > pIndexBuffer ) const
    {
        m_pImmediateCommandList->BindIndexBuffer( pIndexBuffer->Handle );
    }

    void RenderSystem::UnbindIndexBuffer() const
    {
        m_pImmediateCommandList->UnbindIndexBuffer();
    }

    void RenderSystem::BindConstantBuffer( const memory::Ref< UniformBuffer > &pUniformBuffer ) const
    {
    }

    void RenderSystem::UnbindConstantBuffer() const
    {
    }

    void RenderSystem::BindShader( memory::Ref< Shader > pShader )
    {
        m_pImmediateCommandList->BindShader( pShader );
        m_pBoundShader = pShader;
    }

    void RenderSystem::UnbindShader()
    {
        m_pImmediateCommandList->UnbindShader();
        m_pBoundShader = nullptr;
    }

    void RenderSystem::BindFramebuffer( memory::Ref< Framebuffer > pFramebuffer )
    {
        m_pImmediateCommandList->BindFramebuffer( pFramebuffer->Handle );
        m_pBoundFramebuffer = pFramebuffer;
    }

    void RenderSystem::BindBackBuffer()
    {
        m_pImmediateCommandList->BindBackBuffer( m_pSwapChain );
        m_pBoundFramebuffer = nullptr;
    }

    void RenderSystem::SetState( const RenderState &state ) const
    {
        m_pImmediateCommandList->SetState( state );
    }

    void *RenderSystem::ReadTexture( memory::Ref< Texture > pTexture ) const
    {
        return m_pImmediateCommandList->ReadTexture( pTexture->Handle );
    }

    void *RenderSystem::ReadTexture( memory::Ref< Framebuffer > pFramebuffer, Uint32 index ) const
    {
        return m_pImmediateCommandList->ReadTexture( pFramebuffer->Handle, index );
    }

    void RenderSystem::DrawIndexed( Uint32 indexCount )
    {
        m_pImmediateCommandList->DrawIndexed( indexCount, m_pBoundShader );
    }

    void RenderSystem::Draw( Uint32 vertexCount )
    {
        m_pImmediateCommandList->Draw( vertexCount, m_pBoundShader );
    }

    void RenderSystem::Present()
    {
        m_pSwapChain->Present();
    }
}