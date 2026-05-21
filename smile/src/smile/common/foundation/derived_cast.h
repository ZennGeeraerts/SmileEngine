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
 * @file        derived_cast.h
 * @author      Zenn Geeraerts
 * @created     19 May 2026
 * @brief       Safe downcast utility
 */

#pragma once

#include <type_traits>

namespace smile::foundation
{
    template < typename ChildRef, typename ParentRef >
    requires( std::is_reference_v< ChildRef > && std::is_reference_v< ParentRef > )
    inline ChildRef DerivedCast( ParentRef &&parent )
    {
        SM_ASSERT( static_cast< typename std::remove_reference_t< ChildRef >::Type * >( &parent ) ==
                   dynamic_cast< typename std::remove_reference_t< ChildRef >::Type * >( &parent ) );

        return static_cast< ChildRef >( parent );
    }
}