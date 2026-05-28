
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
 * @file        recursivity.h
 * @author      Zenn Geeraerts
 * @created     28 May 2026
 * @brief       Enum for specifying recursive or non-recursive filesystem operations.
 */

#pragma once

namespace smile::fs
{
    enum class Recursivity
    {
        Recursive,
        NonRecursive
    };
}