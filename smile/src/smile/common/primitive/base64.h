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
 * @file        base64.h
 * @author      Zenn Geeraerts
 * @created     29 May 2026
 * @brief       Base64 encoding and decoding.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "collection/vector.h"
#include "text/string_view.h"

namespace smile::stream
{
    class CharStream;
}

namespace smile::primitive
{
    class Base64 final
    {
      public:
        static void Encode( stream::CharStream &encoded, const primitive::Vector< Byte > &decoded );
        static bool Decode( primitive::Vector< Byte > &decoded, const primitive::StringView encoded );
    };
}