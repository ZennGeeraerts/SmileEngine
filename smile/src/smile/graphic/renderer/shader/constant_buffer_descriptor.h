/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/buffer.h"

#include <string>
#include <vector>

namespace smile::graphic
{
    struct ConstantBufferDescriptor final
    {
        std::string Name;
        Uint32 Size;
        VertexLayout Layout;
    };
}