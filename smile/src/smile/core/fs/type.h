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
 * @file        type.h
 * @author      Zenn Geeraerts
 * @created     17 December 2025
 * @brief       File type
 */
#pragma once

#include "smile/common/primitive/text/string_view.h"

namespace smile::fs
{
    enum class Type
    {
        File,
        Directory,
        Count,
        None
    };

    primitive::StringView GetName( Type type );
    Type GetTypeFromName( const primitive::StringView name );
}