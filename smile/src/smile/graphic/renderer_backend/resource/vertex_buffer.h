/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "memory/object.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    struct VertexBuffer final : public memory::Object
    {
        VertexBuffer( VertexBufferHandle handle, Uint32 stride ) : Handle{ handle }, Stride{ stride }
        {
        }
        ~VertexBuffer() = default;

        VertexBufferHandle Handle;
        Uint32 Stride = 0;
    };
}