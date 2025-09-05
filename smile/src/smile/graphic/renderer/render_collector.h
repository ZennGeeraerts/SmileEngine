/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource/vertex_buffer.h"
#include "resource/index_buffer.h"
#include "shader/vertex_shader.h"
#include "shader/pixel_shader.h"

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawCommand final
    {
        VertexBuffer::Ref pVertexBuffer;
        IndexBuffer::Ref pIndexBuffer;
        VertexShader::Ref pVertexShader;
        PixelShader::Ref pPixelShader;
        DirectX::XMFLOAT4X4 WorldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 ViewInverseMatrix;
        DirectX::XMFLOAT4X4 ViewProjectionMatrix;

        primitive::Vector< DrawCommand > DrawList;
    };
}