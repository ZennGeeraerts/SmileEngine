/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/buffer.h"
#include "smile/graphic/renderer_backend/render_handle.h"

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