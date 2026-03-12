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
 * @file        utils.cpp
 * @author      Zenn Geeraerts
 * @created     12 March 2026
 * @brief       Windows platform utils
 */
#include "smpch.h"
#include "utils.h"

namespace smile::platform
{
    const char *LPTSTRToCString( LPTSTR lptstr )
    {
        if ( !lptstr )
            return nullptr;

#ifdef UNICODE
        const int sizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lptstr, -1, nullptr, 0, nullptr, nullptr );
        if ( sizeNeeded <= 0 )
            return nullptr;

        char *buffer = new char[sizeNeeded];

        WideCharToMultiByte( CP_UTF8, 0, lptstr, -1, buffer, sizeNeeded, nullptr, nullptr );

        return buffer;
#else
        Count len = strlen( lptstr ) + 1;
        char *buffer = new char[len];
        strcpy_s( buffer, len, lptstr );
        return buffer;
#endif
    }
}