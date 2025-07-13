/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_stage.h"

namespace smile::graphic::rhi
{
    struct ShaderDescriptor final
    {
        ShaderDescriptor( ShaderStage stage ) : Stage{ stage }
        {
        }

        ShaderStage Stage = ShaderStage::Unknown;
        std::string EntryPoint = "main";
        std::string TargetProfile;
    };
}
