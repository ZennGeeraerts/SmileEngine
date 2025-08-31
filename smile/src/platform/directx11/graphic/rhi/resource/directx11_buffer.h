/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/graphics_device.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Buffer final
    {
        DirectX11Buffer() = default;
        ~DirectX11Buffer() = default;

        void Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc );
        void Destroy();

        ID3D11Buffer *pInternal = nullptr;
    };
}