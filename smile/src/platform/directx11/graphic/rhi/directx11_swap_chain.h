/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/rhi/swap_chain.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    class DirectX11SwapChain final : public SwapChain
    {
      public:
        DirectX11SwapChain( const window::Window *pWindow,
            ID3D11Device *pDevice,
            ID3D11DeviceContext *pDeviceContext,
            IDXGIFactory *pDXGIFactory );
        ~DirectX11SwapChain();

        void Create();
        void Destroy();

        void Present() override;
        void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

        inline ID3D11RenderTargetView *GetRenderTargetView() const
        {
            return m_pCurrentRenderTarget;
        }
        inline ID3D11DepthStencilView *GetDepthStencilView() const
        {
            return m_pDepthStencilView;
        }
        inline const D3D11_VIEWPORT &GetViewport() const
        {
            return m_Viewport;
        }

      protected:
        IDXGISwapChain *m_pSwapChain = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        ID3D11DepthStencilView *m_pDepthStencilView;
        ID3D11Texture2D *m_pDepthStencilTexture;

        D3D11_VIEWPORT m_Viewport{};

        ID3D11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pDeviceContext = nullptr;
        IDXGIFactory *m_pDXGIFactory = nullptr;

        friend class DirectX11Device;
    };
}