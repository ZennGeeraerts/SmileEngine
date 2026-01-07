/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <d3d11.h>

namespace smile::graphic::rhi
{
    struct D3D11Context final
    {
        D3D11Context() = default;

        D3D11Context( const D3D11Context & ) = delete;
        D3D11Context( D3D11Context && ) = delete;
        D3D11Context &operator=( const D3D11Context & ) = delete;
        D3D11Context &operator=( D3D11Context && ) = delete;

        ID3D11Device *pDevice = nullptr;
        ID3D11DeviceContext *pImmediateContext = nullptr;
        IDXGIFactory *pDXGIFactory = nullptr;
    };
}