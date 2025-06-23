/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/shader/shader.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11Shader final
    {
        DirectX11Shader() = default;
        ~DirectX11Shader();

        DirectX11Shader( const DirectX11Shader & ) = delete;
        DirectX11Shader( DirectX11Shader && ) = delete;
        DirectX11Shader &operator=( const DirectX11Shader & ) = delete;
        DirectX11Shader &operator=( DirectX11Shader && ) = delete;

        void Create( ID3D11Device *pDevice, const ShaderDescriptor &desc, const std::vector< Byte > &byteCode );
        void Destroy();

        union
        {
            ID3D11VertexShader *pVertexShader;
            ID3D11PixelShader *pPixelShader;
            ID3D11DeviceChild *pPtr;
        };

        std::vector< Byte > ByteCode;
        ShaderDescriptor Descriptor;
    };
}
