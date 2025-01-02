/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#pragma once

#include "smile/engine/graphic/renderer_api/resource/rasterizer_state.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11RasterizerState final : public RasterizerState
    {
        DirectX11RasterizerState() = default;
        virtual ~DirectX11RasterizerState();

        DirectX11RasterizerState( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState( DirectX11RasterizerState && ) = delete;
        DirectX11RasterizerState &operator=( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState &operator=( DirectX11RasterizerState && ) = delete;

        void *GetInternal() const override
        {
            return pInternal;
        }

        ID3D11RasterizerState *pInternal = nullptr;
    };
}