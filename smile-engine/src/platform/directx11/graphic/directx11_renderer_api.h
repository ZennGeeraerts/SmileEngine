/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api.h"
#include "directx11_swap_chain.h"

namespace smile::graphic
{
    class DirectX11RendererAPI final : public RendererAPI
    {
      public:
        DirectX11RendererAPI() = default;
        ~DirectX11RendererAPI();
        DirectX11RendererAPI( const DirectX11RendererAPI & ) = delete;
        DirectX11RendererAPI( DirectX11RendererAPI && ) = delete;

        void Initialize( window::Window *pWindow ) override;

        void Draw( Uint32 vertexCount, const Ref< Shader > &pShader ) override;
        void DrawIndexed( Uint32 indexCount, const Ref< Shader > &pShader ) override;
        void Present() override;
        void Clear() override;

        void ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height ) override;

      private:
        DirectX11SwapChain *m_pSwapChain = nullptr;

        IDXGIFactory *m_pDXGIFactory = nullptr;
    };
}
