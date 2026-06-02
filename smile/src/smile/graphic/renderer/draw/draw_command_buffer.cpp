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
 * @file        draw_command_buffer.cpp
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       DrawCommandBuffer — Sort() implementation.
 */
#include "smpch.h"
#include "smile/graphic/renderer/draw/draw_command_buffer.h"

#include <algorithm>

namespace smile::graphic
{
    void DrawCommandBuffer::Sort()
    {
        if ( m_Sorted )
            return;

        std::sort( m_Commands.begin(),
            m_Commands.end(),
            []( const DrawCommand &a, const DrawCommand &b ) noexcept { return a.Key < b.Key; } );

        m_Sorted = true;
    }
}
