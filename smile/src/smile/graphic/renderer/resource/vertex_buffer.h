/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer_backend/render_handle.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"

namespace smile::graphic
{
    struct VertexBuffer final : public memory::Counted
    {
        VertexBuffer( GPUBufferHandle handle, const VertexLayout &layout ) : Handle{ handle }, Layout{ layout }
        {
        }
        ~VertexBuffer() = default;

        GPUBufferHandle Handle;
        VertexLayout Layout;
    };
}