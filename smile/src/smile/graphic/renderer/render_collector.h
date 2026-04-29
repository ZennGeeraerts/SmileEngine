/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "resource/vertex_buffer.h"
#include "resource/index_buffer.h"
#include "material/material_instance.h"

#include "smile/common/primitive/collection/vector.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawItem final
    {
        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
        MaterialInstance::Ref MaterialInstance;
        DirectX::XMFLOAT4X4 WorldTransform;
    };

    struct RenderCollector final
    {
        primitive::Vector< DrawItem > DrawList;
    };
}