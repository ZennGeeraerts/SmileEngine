/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_backend/renderer_backend.h"

namespace smile::graphic
{
    class DirectX11RendererBackend final : public RendererBackend
    {
      public:
        DirectX11RendererBackend();
        ~DirectX11RendererBackend();
        DirectX11RendererBackend( const DirectX11RendererBackend & ) = delete;
        DirectX11RendererBackend( DirectX11RendererBackend && ) = delete;
    };
}
