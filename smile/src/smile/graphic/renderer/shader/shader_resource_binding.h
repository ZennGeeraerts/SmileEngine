/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/shader/shader_data_type.h"

namespace smile::graphic
{
    struct ShaderResourceBinding final
    {
        std::string Name;
        ShaderDataType Type;
        Uint32 BindPoint;
        Uint32 BindCount;
    };
}