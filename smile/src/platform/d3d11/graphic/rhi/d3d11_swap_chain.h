/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/rhi/swap_chain.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    class D3D11SwapChain final : public SwapChain
    {
      public:
        D3D11SwapChain( const window::Window *pWindow,
            ID3D11Device *pDevice,
            ID3D11DeviceContext *pDeviceContext,
            IDXGIFactory *pDXGIFactory );
        ~D3D11SwapChain();

        void Create();
        void Destroy();

        void Present() const override;
        void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

        Object GetNativeRenderTarget() const override;

        inline const D3D11_VIEWPORT &GetViewport() const
        {
            return m_Viewport;
        }

      protected:
        IDXGISwapChain *m_pSwapChain = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        D3D11_VIEWPORT m_Viewport{};

        ID3D11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pDeviceContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;

        friend class D3D11Device;
    };
}