/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/rhi/render_state.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    struct D3D11DepthStencilState final
    {
        D3D11DepthStencilState() = default;
        ~D3D11DepthStencilState();

        D3D11DepthStencilState( const D3D11DepthStencilState & ) = delete;
        D3D11DepthStencilState( D3D11DepthStencilState && ) = delete;
        D3D11DepthStencilState &operator=( const D3D11DepthStencilState & ) = delete;
        D3D11DepthStencilState &operator=( D3D11DepthStencilState && ) = delete;

        void Create( ID3D11Device *pDevice, const DepthStencilState &depthStencilState );
        void Destroy();

        ID3D11DepthStencilState *pInternal = nullptr;
    };
}