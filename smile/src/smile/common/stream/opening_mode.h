/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/flags.h"

namespace smile::stream
{
    enum class OpeningMode
    {
        Truncate,
        Append
    };

    using OpeningModeFlags = foundation::Flags< OpeningMode >;
}