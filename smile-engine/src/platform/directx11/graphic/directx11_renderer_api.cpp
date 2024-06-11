/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_renderer_api.h"

#include "platform/directx11/graphic/shader/directx11_shader.h"

#include "directx11_diagnostics.h"

#include "smile_engine/graphic/render_engine.h"

namespace smile::graphic
{
    void DirectX11RendererAPI::Initialize()
    {
        m_pDirectX11Device = static_cast< DirectX11Device * >( RenderEngine::GetDevice() );
        SM_ASSERT( m_pDirectX11Device, "DirectX11RendererAPI > GraphicsDevice is not a DirectX11Device" );

        m_pDirectX11Context = static_cast< DirectX11Context * >( RenderEngine::GetContext() );
        SM_ASSERT( m_pDirectX11Context, "DirectX11RendererAPI > GraphicsContext is not a DirectX11Context" );
    }

    void DirectX11RendererAPI::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        if ( !m_pDirectX11Context->m_pSwapChain || !m_pDirectX11Device->m_pInternal ||
             !m_pDirectX11Context->m_pInternal )
            return;

        m_pDirectX11Context->m_pInternal->OMSetRenderTargets( 0, 0, 0 );

        D3D11_TEXTURE2D_DESC depthStencilDesc{};
        m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilAttachment->GetDesc( &depthStencilDesc );
        depthStencilDesc.Width = width;
        depthStencilDesc.Height = height;

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
        m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView->GetDesc( &depthStencilViewDesc );

        SAFE_RELEASE( m_pDirectX11Context->m_pCurrentRenderTarget );
        SAFE_RELEASE( m_pDirectX11Context->m_pRenderTargetBuffer );
        SAFE_RELEASE( m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView );
        SAFE_RELEASE( m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilAttachment );

        HRESULT result = m_pDirectX11Context->m_pSwapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::ResizeWindow > Failed to resize buffers: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        // Depth stencil
        result = m_pDirectX11Device->m_pInternal->CreateTexture2D(
            &depthStencilDesc, 0, &m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilAttachment );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::ResizeWindow > Failed to create depth stencil buffer: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        result = m_pDirectX11Device->m_pInternal->CreateDepthStencilView(
            m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilAttachment,
            &depthStencilViewDesc,
            &m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::ResizeWindow > Failed to create depth stencil view: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        // Render target
        result = m_pDirectX11Context->m_pSwapChain->GetBuffer( 0,
            __uuidof( ID3D11Texture2D ),
            reinterpret_cast< void ** >( &m_pDirectX11Context->m_pRenderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::ResizeWindow > Failed to get buffer from swap chain: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        result = m_pDirectX11Device->m_pInternal->CreateRenderTargetView(
            m_pDirectX11Context->m_pRenderTargetBuffer, 0, &m_pDirectX11Context->m_pCurrentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11RendererAPI::ResizeWindow > Failed to create render target view: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        m_pDirectX11Context->m_pInternal->OMSetRenderTargets( 1,
            &m_pDirectX11Context->m_pCurrentRenderTarget,
            m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView );

        m_pDirectX11Context->m_Viewport.Width = static_cast< FLOAT >( width );
        m_pDirectX11Context->m_Viewport.Height = static_cast< FLOAT >( height );
        m_pDirectX11Context->m_Viewport.TopLeftX = static_cast< FLOAT >( x );
        m_pDirectX11Context->m_Viewport.TopLeftY = static_cast< FLOAT >( y );

        m_pDirectX11Context->m_pInternal->RSSetViewports( 1, &m_pDirectX11Context->m_Viewport );
    }

    void DirectX11RendererAPI::SetClearColor( const DirectX::XMFLOAT4 &color )
    {
        m_ClearColor = color;
    }

    void DirectX11RendererAPI::Clear()
    {
        m_pDirectX11Context->m_pInternal->OMSetRenderTargets( 1,
            &m_pDirectX11Context->m_pCurrentRenderTarget,
            m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView );

        const float *pClearColor = reinterpret_cast< const float * >( &m_ClearColor );
        m_pDirectX11Context->m_pInternal->ClearRenderTargetView(
            m_pDirectX11Context->m_pCurrentRenderTarget, pClearColor );
        m_pDirectX11Context->m_pInternal->ClearDepthStencilView(
            m_pDirectX11Context->m_pSwapChainTarget->pDepthStencilView,
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
            1.f,
            0 );
    }

    void DirectX11RendererAPI::DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = static_cast< DirectX11Shader * >( pShader.get() );
        SM_ASSERT( pDirectX11Shader, "DirectX11RendererAPI::DrawIndexed > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_pDirectX11Context->m_pInternal );
            m_pDirectX11Context->m_pInternal->DrawIndexed( indexCount, 0, 0 );
        }
    }

    void DirectX11RendererAPI::Draw( Uint32 vertexCount, const Ref< Shader > &pShader )
    {
        auto pDirectX11Shader = static_cast< DirectX11Shader * >( pShader.get() );
        SM_ASSERT( pDirectX11Shader, "DirectX11RendererAPI::Draw > Shader is not a DirectX11Shader" );

        auto pTechnique = pDirectX11Shader->pTechnique;
        D3DX11_TECHNIQUE_DESC techDesc{};
        pTechnique->GetDesc( &techDesc );
        for ( UINT p{}; p < techDesc.Passes; ++p )
        {
            pTechnique->GetPassByIndex( p )->Apply( 0, m_pDirectX11Context->m_pInternal );
            m_pDirectX11Context->m_pInternal->Draw( vertexCount, 0 );
        }
    }
}