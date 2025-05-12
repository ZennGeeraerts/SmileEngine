/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/graphics_device.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Buffer final
    {
        DirectX11Buffer() = default;
        ~DirectX11Buffer() = default;

        void Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc );
        void Destroy();

        ID3D11ShaderResourceView *GetOrCreateShaderResourceView( ID3D11Device *pDevice,
            Format format,
            BufferRange bufferRange,
            ResourceType type );

        ID3D11UnorderedAccessView *GetOrCreateUnorderedAccessView( ID3D11Device *pDevice,
            Format format,
            BufferRange bufferRange,
            ResourceType type );

        ID3D11Buffer *pInternal = nullptr;
        GPUBufferDescriptor Descriptor;
        std::unordered_map< BufferBindingKey, ID3D11ShaderResourceView * > ShaderResourceViewMap;
        std::unordered_map< BufferBindingKey, ID3D11UnorderedAccessView * > UnorderedAccessViewMap;
    };
}