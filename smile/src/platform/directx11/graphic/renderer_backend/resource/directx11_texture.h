/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/texture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic
{
    class DirectX11Texture final
    {
      public:
        DirectX11Texture() = default;
        ~DirectX11Texture() = default;

        DirectX11Texture( const DirectX11Texture & ) = delete;
        DirectX11Texture( DirectX11Texture && ) = delete;
        DirectX11Texture &operator=( const DirectX11Texture & ) = delete;
        DirectX11Texture &operator=( DirectX11Texture && ) = delete;

        void Create( ID3D11Device *pDevice, const TextureDescriptor &desc, const std::vector< Byte > &buffer );
        void Destroy();

        ID3D11ShaderResourceView *GetOrCreateShaderResourceView( ID3D11Device *pDevice,
            Format format,
            TextureSubresourceSet subresources,
            TextureDimension dimension );

        Microsoft::WRL::ComPtr< ID3D11Resource > pInternal;
        TextureDescriptor Descriptor;

      private:
        template < typename Type >
        using TextureBindingMap = std::unordered_map< TextureBindingKey, Microsoft::WRL::ComPtr< Type > >;

        TextureBindingMap< ID3D11ShaderResourceView > m_ShaderResourceViewMap;
        TextureBindingMap< ID3D11RenderTargetView > m_RenderTargetViewMap;
        TextureBindingMap< ID3D11DepthStencilView > m_DepthStencilViewMap;
        TextureBindingMap< ID3D11UnorderedAccessView > m_UnorderedAccessViewMap;
    };
}
