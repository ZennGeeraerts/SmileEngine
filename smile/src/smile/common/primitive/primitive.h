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
 * @file        primitive.h
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Contains primitive helper functions
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/compiler.h"
#include "smile/common/platform/platform_detection.h"

#ifdef SM_PLATFORM_WINDOWS
#    include <windows.h>
#endif

namespace smile::primitive
{
    inline Count CountTrailingZeroBits( const Uint32 value )
    {
#if defined( SM_COMPILER_GNU ) || defined( SM_COMPILER_CLANG )
        return value == 0 ? 32 : __builtin_ctz( value );
#elif defined SM_COMPILER_MSVC
        DWORD trailingZero{ 0 };

        if ( _BitScanForward( &trailingZero, value ) )
        {
            return trailingZero;
        }
        else
        {
            return 32;
        }
#else
#    error "Unsupported compiler"
#endif
    }

    inline Count CountTrailingOneBits( const Uint32 value )
    {
        return CountTrailingZeroBits( ~value );
    }
}