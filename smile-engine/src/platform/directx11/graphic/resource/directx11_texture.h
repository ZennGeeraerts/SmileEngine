/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api/resource/texture.h"

#include <DirectXTex.h>

namespace smile::graphic
{
    struct DirectX11Texture : public Texture
    {
        DirectX11Texture() = default;
        virtual ~DirectX11Texture();

        DirectX11Texture( const DirectX11Texture & ) = delete;
        DirectX11Texture( DirectX11Texture && ) = delete;
        DirectX11Texture &operator=( const DirectX11Texture & ) = delete;
        DirectX11Texture &operator=( DirectX11Texture && ) = delete;

        void *GetData() const override
        {
            return pShaderResourceView;
        }

        ID3D11Resource *pTexture = nullptr;
        ID3D11ShaderResourceView *pShaderResourceView = nullptr;
    };

    struct DirectX11Texture2D final : public DirectX11Texture
    {
        DirectX11Texture2D() = default;
    };

    struct DirectX11TextureCube final : public DirectX11Texture
    {
        DirectX11TextureCube() = default;
    };
}
