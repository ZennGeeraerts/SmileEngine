/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::graphic
{
    enum class ConstantType
    {
        Unknown,
        Float,
        Float2,
        Float3,
        Int,
        Bool,
        Mat4,
        Mat4Array
    };

    Count GetSize( ConstantType type );
}