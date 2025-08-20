/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "math_utilities.h"

namespace smile::math
{
    using Angle = float;

    inline Angle GetFromDegrees( const Angle degrees )
    {
        return degrees * g_PI / 180.0f;
    }
}