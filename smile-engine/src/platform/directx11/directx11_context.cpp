#include "smpch.h"
#include "directx11_context.h"

#include "directx11_device.h"
#include "directx11_diagnostics.h"

#include "smile_engine/core/window.h"
#include "smile_engine/core/core.h"
#include "smile_engine/core/logger.h"

#include "platform/directx11/resource/directx11_frame_buffer.h"

namespace smile::graphic
{
    DirectX11Context::DirectX11Context( Window *pWindow ) : m_pWindow{ pWindow }
    {
    }

    DirectX11Context::~DirectX11Context()
    {
        //SAFE_RELEASE( m_pDepthStencilBuffer );
        SAFE_RELEASE( m_pRenderTargetBuffer );
        SAFE_RELEASE( m_pCurrentRenderTarget );
        //SAFE_RELEASE( m_pDepthStencilView );
        SAFE_RELEASE( m_pDXGIFactory );
        SAFE_RELEASE( m_pSwapChain );

        if ( m_pInternal )
        {
            m_pInternal->ClearState();
            m_pInternal->Flush();
            SAFE_RELEASE( m_pInternal );
        }
    }

    void DirectX11Context::Initialize( GraphicsDevice *pGraphicsDevice )
    {
        ID3D11Device *pDevice = static_cast< ID3D11Device * >( pGraphicsDevice->GetInternal() );

        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::Initialize > Failed to create DXGIFactory: %ls", GetDirectX11ErrorMessage( result ) );
            return;
        }

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
        result = m_pDXGIFactory->CreateSwapChain( pDevice, &swapChainDesc, &m_pSwapChain );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::Initialize > Failed to create swap chain: %ls", GetDirectX11ErrorMessage( result ) );
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

        m_pSwapChainTarget =
            std::static_pointer_cast< DirectX11Framebuffer >( pGraphicsDevice->CreateFramebuffer( framebufferDesc ) );

        // Create the RenderTargetView
        result = m_pSwapChain->GetBuffer(
            0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void ** >( &m_pRenderTargetBuffer ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectXContext::Initialize > Failed to get buffer from swap chain: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        result = pDevice->CreateRenderTargetView( m_pRenderTargetBuffer, 0, &m_pCurrentRenderTarget );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectXContext::Initialize > Failed to create render target view: %ls",
                GetDirectX11ErrorMessage( result ) );
            return;
        }

        m_pInternal->OMSetRenderTargets( 1, &m_pCurrentRenderTarget, m_pSwapChainTarget->pDepthStencilView );
        /*------------------------------------- Render Target Code End -------------------------------------*/

        // Set the Viewport
        m_Viewport = {};
        m_Viewport.Width = static_cast< FLOAT >( width );
        m_Viewport.Height = static_cast< FLOAT >( height );
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0;
        m_pInternal->RSSetViewports( 1, &m_Viewport );
    }

    void DirectX11Context::Present()
    {
        m_pSwapChain->Present( m_pWindow->IsVSync(), 0 );
    }

    void DirectX11Context::BindVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) const
    {
        Uint32 offset{ 0 };
        ID3D11Buffer *pInternalBuffer = static_cast< ID3D11Buffer * >( pVertexBuffer->GetInternal() );
        m_pInternal->IASetVertexBuffers( 0, 1, &pInternalBuffer, &pVertexBuffer->Stride, &offset );
    }

    void DirectX11Context::UnbindVertexBuffer() const
    {
        m_pInternal->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }

    void DirectX11Context::BindIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) const
    {
        ID3D11Buffer *pInternalBuffer = static_cast< ID3D11Buffer * >( pIndexBuffer->GetInternal() );
        m_pInternal->IASetIndexBuffer( pInternalBuffer, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11Context::UnbindIndexBuffer() const
    {
        m_pInternal->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }

    void DirectX11Context::BindShader( const Ref< Shader > &pShader ) const
    {
        ID3D11InputLayout *pInputLayout = static_cast< ID3D11InputLayout * >( pShader->GetData() );
        m_pInternal->IASetInputLayout( pInputLayout );
    }

    void DirectX11Context::UnbindShader() const
    {
        m_pInternal->IASetInputLayout( nullptr );
    }

    void DirectX11Context::BindFramebuffer( const Ref< Framebuffer > &pFramebuffer ) const
    {
        auto pRenderTargetViews =
            static_cast< ID3D11RenderTargetView *const * >( pFramebuffer->GetRenderTargetViews() );
        auto pDepthStencilView = static_cast< ID3D11DepthStencilView * >( pFramebuffer->GetDepthStencilView() );
        auto pViewport = static_cast< D3D11_VIEWPORT * >( pFramebuffer->GetViewport() );

        m_pInternal->OMSetRenderTargets(
            pFramebuffer->GetRenderTargetViewCount(), &pRenderTargetViews[0], pDepthStencilView );
        m_pInternal->RSSetViewports( 1, pViewport );
    }

    void DirectX11Context::UnbindFramebuffer() const
    {
        m_pInternal->OMSetRenderTargets( 1, &m_pCurrentRenderTarget, m_pSwapChainTarget->pDepthStencilView );
        m_pInternal->RSSetViewports( 1, &m_Viewport );
    }

    void DirectX11Context::ClearFramebuffer( const Ref< Framebuffer > &pFramebuffer )
    {
        const float *pClearColor = reinterpret_cast< const float * >( &pFramebuffer->ClearColor );
        auto pRenderTargetViews =
            static_cast< ID3D11RenderTargetView *const * >( pFramebuffer->GetRenderTargetViews() );
        auto pDepthStencilView = static_cast< ID3D11DepthStencilView * >( pFramebuffer->GetDepthStencilView() );

        for ( Uint32 i{}; i < pFramebuffer->GetRenderTargetViewCount(); ++i )
        {
            m_pInternal->ClearRenderTargetView( pRenderTargetViews[i], pClearColor );
        }

        if ( pFramebuffer->DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
            m_pInternal->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0 );
    }

    void DirectX11Context::BindRasterizerState( const Ref< RasterizerState > &pRasterizerState ) const
    {
        ID3D11RasterizerState *pD11RasterizerState =
            static_cast< ID3D11RasterizerState * >( pRasterizerState->GetInternal() );
        m_pInternal->RSSetState( pD11RasterizerState );
    }

    void DirectX11Context::UnbindRasterizerState() const
    {
        m_pInternal->RSSetState( nullptr );
    }
}