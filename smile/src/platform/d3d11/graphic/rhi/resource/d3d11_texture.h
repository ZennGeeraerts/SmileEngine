/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/texture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic::rhi
{
    class D3D11Texture final
    {
      public:
        D3D11Texture() = default;
        ~D3D11Texture();

        D3D11Texture( const D3D11Texture & ) = delete;
        D3D11Texture( D3D11Texture && ) = delete;
        D3D11Texture &operator=( const D3D11Texture & ) = delete;
        D3D11Texture &operator=( D3D11Texture && ) = delete;

        void Create( ID3D11Device *pDevice, const TextureDescriptor &desc, const std::vector< Byte > &buffer );
        void Destroy();

        ID3D11ShaderResourceView *GetOrCreateShaderResourceView( ID3D11Device *pDevice,
            Format format,
            TextureSubresourceSet subresources,
            TextureDimension dimension );

        ID3D11RenderTargetView *
        GetOrCreateRenderTargetView( ID3D11Device *pDevice, Format format, TextureSubresourceSet subresources );

        ID3D11DepthStencilView *GetOrCreateDepthStencilView( ID3D11Device *pDevice,
            TextureSubresourceSet subresources,
            bool isReadOnly = false );

        ID3D11UnorderedAccessView *GetOrCreateUnorderedAccessView( ID3D11Device *pDevice,
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
