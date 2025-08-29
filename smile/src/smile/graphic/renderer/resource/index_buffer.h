/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    struct IndexBuffer : public memory::Counted
    {
        IndexBuffer( GPUBufferHandle handle, Uint32 count ) : Handle{ handle }, Count{ count }
        {
        }
        ~IndexBuffer() = default;

        GPUBufferHandle Handle;
        Uint32 Count;
    };
}