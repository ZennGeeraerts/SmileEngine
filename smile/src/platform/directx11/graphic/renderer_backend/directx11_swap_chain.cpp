/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_swap_chain.h"

#include "smile/core/window/window.h"

namespace smile::graphic
{
    DirectX11SwapChain::DirectX11SwapChain( const window::Window *pWindow ) : SwapChain{ pWindow }
    {
    }

    DirectX11SwapChain::~DirectX11SwapChain()
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
}