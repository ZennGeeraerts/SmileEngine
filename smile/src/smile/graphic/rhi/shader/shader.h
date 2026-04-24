/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader_stage.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/handle_manager.h"

namespace smile::graphic::rhi
{
    struct ShaderDescriptor final
    {
        ShaderDescriptor() = default;

        ShaderDescriptor( ShaderStage stage ) : Stage{ stage }
        {
        }

        ShaderStage Stage = ShaderStage::Unknown;
        primitive::String EntryPoint = "main";
        primitive::String TargetProfile;
    };

    using ShaderHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using ShaderHandle = ShaderHandleManager::HandleType;

    static constexpr Uint16 s_MaxShaderCount = ( 4 << 10 );
}
