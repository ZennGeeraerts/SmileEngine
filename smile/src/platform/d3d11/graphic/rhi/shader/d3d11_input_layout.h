/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/rhi/render_handle.h"

class ID3D11Device;
class ID3D11InputLayout;

namespace smile::graphic::rhi
{
    struct D3D11Shader;

    struct D3D11InputLayout final
    {
        D3D11InputLayout() = default;
        ~D3D11InputLayout();

        void Create( ID3D11Device *pDevice, const BufferLayout &layout, const D3D11Shader &vertexShader );
        void Destroy();

        ID3D11InputLayout *pInternal = nullptr;
    };
}