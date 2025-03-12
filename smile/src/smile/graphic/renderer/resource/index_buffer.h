/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/object.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    struct IndexBuffer : public memory::Object
    {
        IndexBuffer( GPUBufferHandle handle, Uint32 count ) : Handle{ handle }, Count{ count }
        {
        }
        ~IndexBuffer() = default;

        GPUBufferHandle Handle;
        Uint32 Count;
    };
}