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
 * @file        type.cpp
 * @author      Zenn Geeraerts
 * @created     17 December 2025
 * @brief       File type
 */
#include "smpch.h"
#include "type.h"

#include "smile/common/primitive/collection/enum_array.h"
#include "smile/common/foundation/enum_value_iterator.h"

namespace smile::fs
{
    primitive::EnumArray< Type, primitive::StringView > g_Names{ "File", "Directory" };

    primitive::StringView GetName( Type type )
    {
        if ( g_Names.IsValidIndex( type ) )
        {
            return g_Names[type];
        }

        return "";
    }

    Type GetTypeFromName( const primitive::StringView name )
    {
        for ( auto type : foundation::EnumValues< Type >() )
        {
            if ( name == g_Names[type] )
            {
                return type;
            }
        }

        return Type::None;
    }
}