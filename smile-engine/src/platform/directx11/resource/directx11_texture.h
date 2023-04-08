#pragma once

#include "smile_engine/graphic/resource/texture.h"

#include <DirectXTex.h>

namespace smile::graphic
{
    struct DirectX11Texture2D final : public Texture2D
    {
        DirectX11Texture2D() = default;
        virtual ~DirectX11Texture2D();

        DirectX11Texture2D( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D( DirectX11Texture2D && ) = delete;
        DirectX11Texture2D &operator=( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D &operator=( DirectX11Texture2D && ) = delete;

        virtual void *GetData() const override
        {
            return pShaderResourceView;
        }

        ID3D11Resource *pTexture = nullptr;
        ID3D11ShaderResourceView *pShaderResourceView = nullptr;
    };
}
