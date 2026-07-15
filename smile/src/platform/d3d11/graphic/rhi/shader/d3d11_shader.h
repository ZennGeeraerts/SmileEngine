/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/shader/shader.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    struct D3D11Shader final
    {
        D3D11Shader() : pPtr{ nullptr }
        {
        }

        ~D3D11Shader();

        D3D11Shader( const D3D11Shader & ) = delete;
        D3D11Shader( D3D11Shader && ) = delete;
        D3D11Shader &operator=( const D3D11Shader & ) = delete;
        D3D11Shader &operator=( D3D11Shader && ) = delete;

        void Create( ID3D11Device *pDevice, const ShaderDescriptor &desc, const primitive::Vector< Byte > &byteCode );
        void Destroy();

        union
        {
            ID3D11VertexShader *pVertexShader;
            ID3D11PixelShader *pPixelShader;
            ID3D11DeviceChild *pPtr;
        };

        primitive::Vector< Byte > ByteCode;
        ShaderDescriptor Descriptor;
    };
}
