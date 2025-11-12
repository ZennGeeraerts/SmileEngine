/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/primitive/text/string.h"
#include "option_base.h"

namespace smile::commandline
{
    struct OptionMap final
    {
        OptionBase *PositionalOption{ nullptr };
        primitive::HashMap< primitive::String, OptionBase * > Options;
        primitive::HashMap< char, OptionBase * > ShortOptions;
    };
}