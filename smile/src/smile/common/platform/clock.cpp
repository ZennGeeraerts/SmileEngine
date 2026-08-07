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
 * @file        clock.cpp
 * @author      Zenn Geeraerts
 * @created     24 July 2026
 * @brief       Provides access to the system clock in microseconds.
 */
#include "smpch.h"
#include "clock.h"

#include <chrono>

namespace smile::platform
{
    const auto g_InitialTimePoint = std::chrono::high_resolution_clock::now();

    SystemTime Clock::GetSystemAbsoluteTime() noexcept
    {
        return std::chrono::duration_cast< std::chrono::microseconds >(
            std::chrono::high_resolution_clock::now().time_since_epoch() )
            .count();
    }

    SystemTime Clock::GetTime() noexcept
    {
        return std::chrono::duration_cast< std::chrono::microseconds >(
            std::chrono::high_resolution_clock::now() - g_InitialTimePoint )
            .count();
    }
}