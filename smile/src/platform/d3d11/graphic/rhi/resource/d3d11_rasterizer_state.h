/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_state.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    struct D3D11RasterizerState final
    {
        D3D11RasterizerState() = default;
        ~D3D11RasterizerState();

        D3D11RasterizerState( const D3D11RasterizerState & ) = delete;
        D3D11RasterizerState( D3D11RasterizerState && ) = delete;
        D3D11RasterizerState &operator=( const D3D11RasterizerState & ) = delete;
        D3D11RasterizerState &operator=( D3D11RasterizerState && ) = delete;

        void Create( ID3D11Device *pDevice, const RasterizerState &rasterizerState );
        void Destroy();

        ID3D11RasterizerState *pInternal = nullptr;
    };
}