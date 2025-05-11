/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/graphic/renderer_backend/render_state.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11DepthStencilState final
    {
        DirectX11DepthStencilState() = default;
        ~DirectX11DepthStencilState();

        DirectX11DepthStencilState( const DirectX11DepthStencilState & ) = delete;
        DirectX11DepthStencilState( DirectX11DepthStencilState && ) = delete;
        DirectX11DepthStencilState &operator=( const DirectX11DepthStencilState & ) = delete;
        DirectX11DepthStencilState &operator=( DirectX11DepthStencilState && ) = delete;

        void Create( ID3D11Device *pDevice, const DepthStencilState &depthStencilState );
        void Destroy();

        ID3D11DepthStencilState *pInternal = nullptr;
    };
}