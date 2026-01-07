/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/graphics_device.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic::rhi
{
    class D3D11Buffer final
    {
      public:
        D3D11Buffer() = default;
        ~D3D11Buffer() = default;

        void Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc, void *pData );
        void Destroy();

        ID3D11ShaderResourceView *GetOrCreateShaderResourceView( ID3D11Device *pDevice,
            Format format,
            BufferRange bufferRange,
            ResourceType type );

        ID3D11UnorderedAccessView *GetOrCreateUnorderedAccessView( ID3D11Device *pDevice,
            Format format,
            BufferRange bufferRange,
            ResourceType type );

        Microsoft::WRL::ComPtr< ID3D11Buffer > pInternal = nullptr;
        GPUBufferDescriptor Descriptor;

      private:
        template < typename Type >
        using BufferBindingMap = std::unordered_map< BufferBindingKey, Microsoft::WRL::ComPtr< Type > >;

        BufferBindingMap< ID3D11ShaderResourceView > m_ShaderResourceViewMap;
        BufferBindingMap< ID3D11UnorderedAccessView > m_UnorderedAccessViewMap;
    };
}