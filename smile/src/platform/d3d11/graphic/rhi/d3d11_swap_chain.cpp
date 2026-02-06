/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_swap_chain.h"

#include "smile/core/window/window.h"
#include "platform/d3dcommon/graphic/d3d_diagnostics.h"

namespace smile::graphic::rhi
{
    D3D11SwapChain::D3D11SwapChain( const window::Window *pWindow,
        ID3D11Device *pDevice,
        ID3D11DeviceContext *pDeviceContext,
        IDXGIFactory *pDXGIFactory )
        : SwapChain{ pWindow }, m_pDevice{ pDevice }, m_pDeviceContext{ pDeviceContext }, m_pDXGIFactory{ pDXGIFactory }
    {
    }

    D3D11SwapChain::~D3D11SwapChain()
    {
        Destroy();
    }

    void D3D11SwapChain::Create()
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
        HRESULT result = m_pDXGIFactory->CreateSwapChain( m_pDevice, &swapChainDesc, &m_pSwapChain );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "D3D11SwapChain::Create > Failed to create swap chain: {}", fmt::ptr( GetD3DErrorMessage( result ) ) );
            return;
        }

        /*------------------------------------- Render Target Code -------------------------------------*/
        // Create the RenderTargetView
        result = m_pSwapChain->GetBuffer(
            0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void ** >( &m_pRenderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11SwapChain::Create > Failed to get buffer from swap chain: {}",
                fmt::ptr( GetD3DErrorMessage( result ) ) );
            return;
        }

        result = m_pDevice->CreateRenderTargetView( m_pRenderTargetBuffer, 0, &m_pCurrentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11SwapChain::Create > Failed to create render target view: {}",
                fmt::ptr( GetD3DErrorMessage( result ) ) );
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

        m_pDeviceContext->RSSetViewports( 1, &m_Viewport );
    }

    void D3D11SwapChain::Destroy()
    {
        SAFE_RELEASE( m_pRenderTargetBuffer );
        SAFE_RELEASE( m_pCurrentRenderTarget );

        SAFE_RELEASE( m_pSwapChain );
    }

    void D3D11SwapChain::Present()
    {
        m_pSwapChain->Present( m_pWindow->IsVSync(), 0 );
    }

    void D3D11SwapChain::Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        SAFE_RELEASE( m_pCurrentRenderTarget );
        SAFE_RELEASE( m_pRenderTargetBuffer );

        HRESULT result = m_pSwapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "D3D11SwapChain::Resize > Failed to resize buffers: {}", fmt::ptr( GetD3DErrorMessage( result ) ) );
            return;
        }

        // Render target
        result = m_pSwapChain->GetBuffer(
            0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void ** >( &m_pRenderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11SwapChain::Resize > Failed to get buffer from swap chain: {}",
                fmt::ptr( GetD3DErrorMessage( result ) ) );
            return;
        }

        result = m_pDevice->CreateRenderTargetView( m_pRenderTargetBuffer, 0, &m_pCurrentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11SwapChain::Resize > Failed to create render target view: {}",
                fmt::ptr( GetD3DErrorMessage( result ) ) );
            return;
        }

        m_Viewport.Width = static_cast< FLOAT >( width );
        m_Viewport.Height = static_cast< FLOAT >( height );
        m_Viewport.TopLeftX = static_cast< FLOAT >( x );
        m_Viewport.TopLeftY = static_cast< FLOAT >( y );

        m_pDeviceContext->RSSetViewports( 1, &m_Viewport );
    }

    Object D3D11SwapChain::GetNativeRenderTarget() const
    {
        return Object{ m_pRenderTargetBuffer };
    }
}