/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/vertex_buffer.h"
#include "smile/graphic/renderer_backend/resource/index_buffer.h"
#include "memory/ref.h"

namespace smile::graphic
{
    struct Mesh final
    {
        memory::Ref< VertexBuffer > pVertexBuffer = nullptr;
        memory::Ref< IndexBuffer > pIndexBuffer = nullptr;
    };
}