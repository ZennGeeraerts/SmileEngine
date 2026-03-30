/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource/vertex_buffer.h"
#include "resource/index_buffer.h"
#include "material/material.h"

#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_state.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawItem final
    {
        VertexBuffer::Ref pVertexBuffer;
        IndexBuffer::Ref pIndexBuffer;
        Material::ConstRef Material;
        DirectX::XMFLOAT4X4 WorldTransform;
        rhi::RenderState RenderState;
    };

    struct RenderCollector final
    {
        primitive::Vector< DrawItem > DrawList;
    };
}