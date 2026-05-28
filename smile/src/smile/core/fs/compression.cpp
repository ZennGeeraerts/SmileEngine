
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
 * @file        compression.cpp
 * @author      Zenn Geeraerts
 * @created     28 May 2026
 * @brief       Provides compression type selection and name conversion utilities.
 */
#include "smpch.h"
#include "compression.h"

#include "smile/common/primitive/collection/enum_array.h"
#include "smile/common/foundation/enum_value_iterator.h"

namespace smile::fs
{
    static primitive::EnumArray< Compression, primitive::StringView > s_CompressionNames{
        "Lz4",
    };

    primitive::StringView GetName( const Compression compression )
    {
        if ( s_CompressionNames.IsValidIndex( compression ) )
        {
            return s_CompressionNames[compression];
        }

        return {};
    }

    void GetFromName( Compression &compression, const primitive::StringView name )
    {
        for ( const Compression currCompression : foundation::EnumValues< Compression >() )
        {
            if ( s_CompressionNames[currCompression] == name )
            {
                compression = currCompression;
                return;
            }
        }

        compression = Compression::None;
    }
}