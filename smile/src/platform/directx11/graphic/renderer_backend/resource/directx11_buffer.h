/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/graphics_device.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Buffer
    {
        DirectX11Buffer() = default;
        virtual ~DirectX11Buffer();

        void Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc, Uint16 bindFlags );
        void Destroy();

        ID3D11Buffer *pInternal = nullptr;
    };

    struct DirectX11VertexBuffer final : public DirectX11Buffer
    {
        DirectX11VertexBuffer() = default;
        ~DirectX11VertexBuffer() = default;

        Uint32 Stride = 0;
    };
}