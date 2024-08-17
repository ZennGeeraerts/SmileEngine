/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api/renderer_api.h"
#include <d3d11.h>

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

      private:
        IDXGIFactory *m_pDXGIFactory = nullptr;
    };
}
