/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/sampler_state.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11SamplerState final
    {
        DirectX11SamplerState() = default;
        ~DirectX11SamplerState();

        DirectX11SamplerState( const DirectX11SamplerState & ) = delete;
        DirectX11SamplerState( DirectX11SamplerState && ) = delete;
        DirectX11SamplerState &operator=( const DirectX11SamplerState & ) = delete;
        DirectX11SamplerState &operator=( DirectX11SamplerState && ) = delete;

        void Create( ID3D11Device *pDevice, const SamplerState &samplerState );
        void Destroy();

        ID3D11SamplerState *pInternal = nullptr;
    };
}