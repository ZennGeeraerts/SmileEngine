/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/rhi/render_handle.h"

class ID3D11Device;
class ID3D11InputLayout;

namespace smile::graphic
{
    struct DirectX11Shader;

    struct DirectX11InputLayout final
    {
        DirectX11InputLayout() = default;
        ~DirectX11InputLayout();

        void Create( ID3D11Device *pDevice, const BufferLayout &layout, const DirectX11Shader &vertexShader );
        void Destroy();

        ID3D11InputLayout *pInternal = nullptr;
    };
}