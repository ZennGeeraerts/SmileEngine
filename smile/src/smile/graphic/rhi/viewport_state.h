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
 * @file        viewport_state.h
 * @author      Zenn Geeraerts
 * @created     7 Januari 2026
 * @brief       Describes the viewport state
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "viewport.h"

namespace smile::graphic::rhi
{
    static constexpr Count s_MaxViewports = 16;

    struct ViewportState final
    {
        primitive::FixedVector< Viewport, s_MaxViewports > Viewports;
        primitive::FixedVector< geometric::Rectangle, s_MaxViewports > ScissorRects;

        inline bool operator==( const ViewportState &other ) const
        {
            return primitive::array::IsEqual( Viewports, other.Viewports ) &&
                   primitive::array::IsEqual( ScissorRects, other.ScissorRects );
        }

        inline bool operator!=( const ViewportState &other ) const
        {
            return !( *this == other );
        }
    };
}