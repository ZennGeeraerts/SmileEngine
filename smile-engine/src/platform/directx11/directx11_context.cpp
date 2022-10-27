#include "smpch.h"
#include "directx11_context.h"

#include "smile_engine/core/window.h"
#include "smile_engine/core/core.h"
#include "smile_engine/core/logger.h"

#include "directx11_diagnostics.h"

namespace smile::graphic
{
    DirectX11Context::DirectX11Context( Window *window ) : window{ window }
    {
    }

    DirectX11Context::~DirectX11Context()
    {
        SAFE_RELEASE( depthStencilBuffer );
        SAFE_RELEASE( renderTargetBuffer );
        SAFE_RELEASE( currentRenderTarget );
        SAFE_RELEASE( depthStencilView );
        SAFE_RELEASE( dxgiFactory );
        SAFE_RELEASE( swapChain );

        if ( deviceContext )
        {
            deviceContext->ClearState();
            deviceContext->Flush();
            SAFE_RELEASE( deviceContext );
        }

        SAFE_RELEASE( device );
    }

    void DirectX11Context::init()
    {
        // Create Device and Device context, using hardware acceleration
        D3D_FEATURE_LEVEL feature_level{ D3D_FEATURE_LEVEL_11_0 };
        Uint32 create_device_flags{ 0 };
#if defined( DEBUG ) || defined( _DEBUG )
        create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        HRESULT result = D3D11CreateDevice( 0,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            create_device_flags,
            0,
            0,
            D3D11_SDK_VERSION,
            &device,
            &feature_level,
            &deviceContext );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::init > Failed to create D3D11Device: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        // Create DXGI Factory to create SwapChain based on hardware
        result = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &dxgiFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::init > Failed to create DXGIFactory: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        const Uint32 width = window->getWidth();
        const Uint32 height = window->getHeight();

        // Create SwapChain Descriptor
        DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
        swap_chain_desc.BufferDesc.Width = width;
        swap_chain_desc.BufferDesc.Height = height;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = 1;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 60;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount = 1;
        swap_chain_desc.Windowed = true;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swap_chain_desc.Flags = 0;

        swap_chain_desc.OutputWindow = static_cast< HWND >( window->getNativeWindow() );

        // Create SwapChain and hook it into the handle of the SDL window
        result = dxgiFactory->CreateSwapChain( device, &swap_chain_desc, &swapChain );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::init > Failed to create swap chain: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        /*------------------------------------- Render Target Code -------------------------------------*/
        // Create the Depth/Stencil Buffer and View
        D3D11_TEXTURE2D_DESC depth_stencil_desc{};
        depth_stencil_desc.Width = width;
        depth_stencil_desc.Height = height;
        depth_stencil_desc.MipLevels = 1;
        depth_stencil_desc.ArraySize = 1;
        depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_stencil_desc.SampleDesc.Count = 1;
        depth_stencil_desc.SampleDesc.Quality = 0;
        depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
        depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_stencil_desc.CPUAccessFlags = 0;
        depth_stencil_desc.MiscFlags = 0;

        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
        depth_stencil_view_desc.Format = depth_stencil_desc.Format;
        depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depth_stencil_view_desc.Texture2D.MipSlice = 0;

        result = device->CreateTexture2D( &depth_stencil_desc, 0, &depthStencilBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectXContext::init > Failed to create depth stencil buffer: %ls",
                getDirectX11ErrorMessage( result ) );
            return;
        }

        result =
            device->CreateDepthStencilView( depthStencilBuffer, &depth_stencil_view_desc, &depthStencilView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::init > Failed to create depth stencil view: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        // Create the RenderTargetView
        result = swapChain->GetBuffer(
            0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void ** >( &renderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectXContext::init > Failed to get buffer from swap chain: %ls",
                getDirectX11ErrorMessage( result ) );
            return;
        }

        result = device->CreateRenderTargetView( renderTargetBuffer, 0, &currentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::init > Failed to create render target view: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }

        deviceContext->OMSetRenderTargets( 1, &currentRenderTarget, depthStencilView );
        /*------------------------------------- Render Target Code End -------------------------------------*/

        // Set the Viewport
        viewport = {};
        viewport.Width = static_cast< FLOAT >( width );
        viewport.Height = static_cast< FLOAT >( height );
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0;
        deviceContext->RSSetViewports( 1, &viewport );
    }

    void DirectX11Context::present()
    {
        swapChain->Present( window->isVSync(), 0 );
    }
}