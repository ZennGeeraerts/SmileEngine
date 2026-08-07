/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        system_time.h
 * @author      Zenn Geeraerts
 * @created     24 July 2026
 * @brief       Represents the system time in microseconds.
 */
#pragma once

#include "smile/common/foundation/compiled.h"

#include <limits>

namespace smile::platform
{
    using SystemTime = Uint64;

    constexpr SystemTime GetMaxSystemTime() noexcept
    {
        return std::numeric_limits< Uint64 >::max();
    }
}