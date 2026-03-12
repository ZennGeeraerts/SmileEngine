/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/sampler.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic::rhi
{
    struct D3D11Sampler final
    {
        D3D11Sampler() = default;
        ~D3D11Sampler();

        D3D11Sampler( const D3D11Sampler & ) = delete;
        D3D11Sampler( D3D11Sampler && ) = delete;
        D3D11Sampler &operator=( const D3D11Sampler & ) = delete;
        D3D11Sampler &operator=( D3D11Sampler && ) = delete;

        void Create( ID3D11Device *pDevice, const SamplerDescriptor &samplerDesc );
        void Destroy();

        Microsoft::WRL::ComPtr< ID3D11SamplerState > pInternal = nullptr;
    };
}