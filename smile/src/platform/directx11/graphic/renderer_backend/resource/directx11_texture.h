/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/texture.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Texture final
    {
        DirectX11Texture() = default;
        ~DirectX11Texture() = default;

        DirectX11Texture( const DirectX11Texture & ) = delete;
        DirectX11Texture( DirectX11Texture && ) = delete;
        DirectX11Texture &operator=( const DirectX11Texture & ) = delete;
        DirectX11Texture &operator=( DirectX11Texture && ) = delete;

        void Create( ID3D11Device *pDevice, const TextureDescriptor &desc, const std::vector< Byte > &buffer );
        void Destroy();

        union
        {
            ID3D11Resource *pInternal;
            ID3D11Texture1D *pTexture1D;
            ID3D11Texture2D *pTexture2D;
            ID3D11Texture3D *pTexture3D;
        };
        TextureDescriptor Descriptor;
        ID3D11ShaderResourceView *pShaderResourceView = nullptr;
    };
}
