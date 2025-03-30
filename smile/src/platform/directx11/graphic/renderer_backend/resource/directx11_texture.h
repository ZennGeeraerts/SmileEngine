/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <d3d11.h>

#include <filesystem>

namespace smile::graphic
{
    struct DirectX11Texture
    {
        DirectX11Texture() = default;
        virtual ~DirectX11Texture();

        DirectX11Texture( const DirectX11Texture & ) = delete;
        DirectX11Texture( DirectX11Texture && ) = delete;
        DirectX11Texture &operator=( const DirectX11Texture & ) = delete;
        DirectX11Texture &operator=( DirectX11Texture && ) = delete;

        void Create( ID3D11Device *pDevice, const std::filesystem::path &path );
        void Destroy();

        union
        {
            ID3D11Resource *pInternal;
            ID3D11Texture2D *pTexture2D;
            ID3D11Texture3D *pTexture3D;
        };

        ID3D11ShaderResourceView *pShaderResourceView = nullptr;
    };
}
