/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/render_state.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11RasterizerState final
    {
        DirectX11RasterizerState() = default;
        ~DirectX11RasterizerState();

        DirectX11RasterizerState( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState( DirectX11RasterizerState && ) = delete;
        DirectX11RasterizerState &operator=( const DirectX11RasterizerState & ) = delete;
        DirectX11RasterizerState &operator=( DirectX11RasterizerState && ) = delete;

        void Create( ID3D11Device *pDevice, const RenderState &renderState );
        void Destroy();

        ID3D11RasterizerState *pInternal = nullptr;
    };
}