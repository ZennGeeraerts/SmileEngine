/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
<<<<<<<< HEAD:smile/src/platform/directx11/graphic/directx11_swap_chain.h
#include "smile/graphic/renderer_api/swap_chain.h"
#include "resource/directx11_frame_buffer.h"
========
#include "engine/graphic/renderer_backend/resource/swap_chain.h"
#include "directx11_frame_buffer.h"
>>>>>>>> main:src/smile/platform/directx11/graphic/renderer_backend/resource/directx11_swap_chain.h

#include "memory/ref.h"

namespace smile::graphic
{
    class DirectX11SwapChain final : public SwapChain
    {
      public:
        DirectX11SwapChain( const window::Window *pWindow );
        ~DirectX11SwapChain();

        void Present() override;

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

        memory::Ref< DirectX11Framebuffer > m_pSwapChainTarget = nullptr;

        ID3D11RenderTargetView *m_pCurrentRenderTarget = nullptr;
        ID3D11Resource *m_pRenderTargetBuffer = nullptr;

        D3D11_VIEWPORT m_Viewport{};

        friend class DirectX11Device;
    };
}