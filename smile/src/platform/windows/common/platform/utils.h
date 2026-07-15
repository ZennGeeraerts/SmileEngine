/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        utils.h
 * @author      Zenn Geeraerts
 * @created     12 March 2026
 * @brief       Windows platform utils
 */
#pragma once

#include <windows.h>

namespace smile::platform
{
    const char *LPTSTRToCString( LPTSTR lptstr );
}