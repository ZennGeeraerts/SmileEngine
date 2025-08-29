/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/graphic/rhi/shader/shader.h"

#include "smile/common/memory/ref.h"

#include <vector>
#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawCommand final
    {
        memory::Ref< VertexBuffer > pVertexBuffer;
        memory::Ref< IndexBuffer > pIndexBuffer;
        memory::Ref< Shader > pShader;
        DirectX::XMFLOAT4X4 WorldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 ViewInverseMatrix;
        DirectX::XMFLOAT4X4 ViewProjectionMatrix;

        std::vector< DrawCommand > DrawList;
    };
}