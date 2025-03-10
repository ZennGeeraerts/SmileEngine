/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/object.h"
#include "smile/graphic/renderer_backend/render_handle.h"

#include <string>

namespace smile::graphic
{
    struct UniformBuffer final : public memory::Object
    {
        UniformBuffer( GPUBufferHandle handle, const std::string &name, Uint32 size )
            : Handle{ handle }, Name{ name }, Size{ size }
        {
        }
        ~UniformBuffer() = default;

        GPUBufferHandle Handle;
        std::string Name;
        Uint32 Size;
    };
}