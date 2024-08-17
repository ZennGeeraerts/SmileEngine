/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_renderer_api.h"

#include "directx11_device.h"
#include "directx11_context.h"
#include "directx11_swap_chain.h"
#include "directx11_diagnostics.h"

namespace smile::graphic
{
    DirectX11RendererAPI::~DirectX11RendererAPI()
    {
        delete m_pDevice;
        delete m_pContext;
        delete m_pSwapChain;

        SAFE_RELEASE( m_pDXGIFactory );
    }

    void DirectX11RendererAPI::Initialize( window::Window *pWindow )
    {
        // Create DXGI Factory to create SwapChain based on hardware
        HRESULT result = CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast< void ** >( &m_pDXGIFactory ) );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectXContext::Initialize > Failed to create DXGIFactory: %ls", GetDirectX11ErrorMessage( result ) );
            return;
        }

        // FIXME
        auto pDirectX11Context = new DirectX11Context{};
        m_pContext = pDirectX11Context;

        m_pDevice = new DirectX11Device{ m_pContext };

        auto pDirectX11SwapChain = new DirectX11SwapChain{ m_pDevice, m_pContext, pWindow, m_pDXGIFactory };
        m_pSwapChain = pDirectX11SwapChain;
        pDirectX11Context->m_pSwapChain = pDirectX11SwapChain;
    }
}