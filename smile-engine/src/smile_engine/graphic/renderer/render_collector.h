/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api/resource/vertex_buffer.h"
#include "smile_engine/graphic/renderer_api/resource/index_buffer.h"
#include "smile_engine/graphic/renderer_api/shader/shader.h"

#include <vector>
#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawCommand final
    {
        Ref< VertexBuffer > pVertexBuffer;
        Ref< IndexBuffer > pIndexBuffer;
        Ref< Shader > pShader;
        DirectX::XMFLOAT4X4 WorldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 ViewInverseMatrix;
        DirectX::XMFLOAT4X4 ViewProjectionMatrix;

        std::vector< DrawCommand > DrawList;
    };
}