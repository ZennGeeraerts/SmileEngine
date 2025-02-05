/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_api/swap_chain.h"
#include "resource/directx11_frame_buffer.h"

namespace smile::graphic
{
    class GraphicsDevice;
    class GraphicsContext;

    class DirectX11SwapChain final : public SwapChain
    {
      public:
        DirectX11SwapChain( GraphicsDevice *pDevice,
            GraphicsContext *pContext,
            window::Window *pWindow,
            IDXGIFactory *pDXGIFactory );
        ~DirectX11SwapChain();

        void Present() override;
        void Resize( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

        inline ID3D11RenderTargetView *GetRenderTargetView() const
        {
            return m_pCurrentRenderTarget;
        }
        inline ID3D11DepthStencilView *GetDepthStencilView() const
        {
            return m_pSwapChainTarget->pDepthStencilView;
        }
        inline const D3D11_VIEWPORT &GetViewport() const
        {
            return m_Viewport;
        }

      protected:
        IDXGISwapChain *m_pSwapChain = nullptr;

        Ref< DirectX11Framebuffer > m_pSwapChainTarget = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        D3D11_VIEWPORT m_Viewport{};

        GraphicsDevice *m_pDevice;
        GraphicsContext *m_pContext;
    };
}