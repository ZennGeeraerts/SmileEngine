/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Context final
    {
        DirectX11Context( const DirectX11Context & ) = delete;
        DirectX11Context( DirectX11Context && ) = delete;
        DirectX11Context &operator=( const DirectX11Context & ) = delete;
        DirectX11Context &operator=( DirectX11Context && ) = delete;

        ID3D11Device *pDevice = nullptr;
        ID3D11DeviceContext *pImmediateContext = nullptr;
        IDXGIFactory *pDXGIFactory = nullptr;
    };
}