/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api/resource/vertex_buffer.h"
#include "smile_engine/graphic/renderer_api/resource/index_buffer.h"

namespace smile::graphic
{
    struct Mesh final
    {
        Ref< VertexBuffer > pVertexBuffer = nullptr;
        Ref< IndexBuffer > pIndexBuffer = nullptr;
    };
}