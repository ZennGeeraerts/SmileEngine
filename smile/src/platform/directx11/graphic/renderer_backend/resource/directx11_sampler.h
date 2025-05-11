/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/sampler.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Sampler final
    {
        DirectX11Sampler() = default;
        ~DirectX11Sampler();

        DirectX11Sampler( const DirectX11Sampler & ) = delete;
        DirectX11Sampler( DirectX11Sampler && ) = delete;
        DirectX11Sampler &operator=( const DirectX11Sampler & ) = delete;
        DirectX11Sampler &operator=( DirectX11Sampler && ) = delete;

        void Create( ID3D11Device *pDevice, const SamplerDescriptor &samplerDesc );
        void Destroy();

        ID3D11SamplerState *pInternal = nullptr;
    };
}