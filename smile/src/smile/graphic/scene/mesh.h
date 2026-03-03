/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"
#include "smile/common/memory/ref.h"

namespace smile::graphic
{
    struct Mesh final
    {
        VertexBuffer::Ref pVertexBuffer = nullptr;
        IndexBuffer::Ref pIndexBuffer = nullptr;
    };
}