/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"
#include "smile/graphic/rhi/shader/resource_type.h"

namespace smile::graphic
{
    struct ShaderResourceBinding final
    {
        primitive::String Name;
        rhi::ResourceType Type;
        Uint32 BindPoint;
        Uint32 BindCount;
    };
}