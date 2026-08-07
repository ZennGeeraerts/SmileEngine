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
 * @file        clock.h
 * @author      Zenn Geeraerts
 * @created     24 July 2026
 * @brief       Provides access to the system clock in microseconds.
 */
#pragma once

#include "system_time.h"

namespace smile::platform
{
    class Clock
    {
      public:
        static SystemTime GetSystemAbsoluteTime() noexcept;
        static SystemTime GetTime() noexcept;
    };
}