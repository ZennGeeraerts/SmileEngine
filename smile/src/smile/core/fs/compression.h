
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
 * @file        compression.h
 * @author      Zenn Geeraerts
 * @created     28 May 2026
 * @brief       Provides compression type selection and name conversion utilities.
 */
#pragma once

#include "smile/common/primitive/text/string_view.h"

namespace smile::fs
{
    enum class Compression
    {
        Lz4,
        Count,
        None
    };

    primitive::StringView GetName( const Compression compression );
    void GetFromName( Compression &compression, const primitive::StringView name );
}