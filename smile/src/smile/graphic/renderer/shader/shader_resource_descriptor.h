/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_data_type.h"

namespace smile::graphic
{
    struct ShaderResourceDescriptor final
    {
        std::string Name;
        ShaderDataType Type;
        Uint32 BindPoint;
        Uint32 BindCount;
    };
}