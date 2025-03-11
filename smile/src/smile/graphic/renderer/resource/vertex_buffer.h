/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "memory/object.h"
#include "smile/graphic/renderer_backend/render_handle.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"

namespace smile::graphic
{
    struct VertexBuffer final : public memory::Object
    {
        VertexBuffer( GPUBufferHandle handle, const VertexLayout &layout ) : Handle{ handle }, Layout{ layout }
        {
        }
        ~VertexBuffer() = default;

        GPUBufferHandle Handle;
        VertexLayout Layout;
    };
}