/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_swap_chain.h"

#include "smile/core/window/window.h"
#include "directx11_diagnostics.h"

namespace smile::graphic
{
    DirectX11SwapChain::DirectX11SwapChain( const window::Window *pWindow ) : SwapChain{ pWindow }
    {
    }

    DirectX11SwapChain::~DirectX11SwapChain()
    {
        Destroy();
    }

    void DirectX11SwapChain::Create( ID3D11Device *pDevice, IDXGIFactory *pDXGIFactory )
    {
        const Uint32 width = m_pWindow->GetWidth();
        const Uint32 height = m_pWindow->GetHeight();

        // Create SwapChain Descriptor
        DXGI_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.BufferDesc.Width = width;
        swapChainDesc.BufferDesc.Height = height;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        swapChainDesc.OutputWindow = static_cast< HWND >( m_pWindow->GetNativeWindow() );

        // Create SwapChain and hook it into the handle of the SDL window
        HRESULT result = pDXGIFactory->CreateSwapChain( pDevice, &swapChainDesc, &m_pSwapChain );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11SwapChain::Create > Failed to create swap chain: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        /*------------------------------------- Render Target Code -------------------------------------*/
        // Create the Depth/Stencil Buffer and View
        FramebufferDescriptor framebufferDesc{};
        framebufferDesc.Width = width;
        framebufferDesc.Height = height;
        framebufferDesc.IsSwapChainTarget = true;
        framebufferDesc.Samples = 1;
        framebufferDesc.Attachments = { { FramebufferTextureFormat::Depth24Stencil8, false } };

        m_SwapChainTarget.Create( pDevice, framebufferDesc );

        // Create the RenderTargetView
        result = m_pSwapChain->GetBuffer(
            0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void ** >( &m_pRenderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11SwapChain::Create > Failed to get buffer from swap chain: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        result = pDevice->CreateRenderTargetView(
            m_pRenderTargetBuffer, 0, &m_pCurrentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11SwapChain::Create > Failed to create render target view: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            return;
        }

        // Set the Viewport
        m_Viewport = {};
        m_Viewport.Width = static_cast< FLOAT >( width );
        m_Viewport.Height = static_cast< FLOAT >( height );
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0;
    }

    void DirectX11SwapChain::Destroy()
    {
        SAFE_RELEASE( m_pRenderTargetBuffer );
        SAFE_RELEASE( m_pCurrentRenderTarget );

        m_SwapChainTarget.Destroy();

        SAFE_RELEASE( m_pSwapChain );
    }

    void DirectX11SwapChain::Present()
    {
        m_pSwapChain->Present( m_pWindow->IsVSync(), 0 );
    }

    void DirectX11SwapChain::Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
    }
}