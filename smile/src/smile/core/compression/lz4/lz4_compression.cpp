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
 * @file        lz4_compression.cpp
 * @author      Zenn Geeraerts
 * @created     9 May 2026
 * @brief       LZ4 compression and decompression utilities
 */
#include "smpch.h"
#include "lz4_compression.h"

#include "smile/common/foundation/numeric_cast.h"

#include <lz4.h>

namespace smile::compression::lz4
{
    BoolResult Compress( const Byte *pData, const Count size, primitive::Vector< Byte > &outCompressed )
    {
        const Count maxDstSize =
            foundation::NumericCast< Count >( LZ4_compressBound( foundation::NumericCast< int >( size ) ) );

        outCompressed.SetItemCount( maxDstSize );

        const int compressedSize = LZ4_compress_default( reinterpret_cast< const char * >( pData ),
            reinterpret_cast< char * >( outCompressed.GetData() ),
            foundation::NumericCast< int >( size ),
            foundation::NumericCast< int >( maxDstSize ) );

        if ( compressedSize <= 0 )
            return BoolResult::Fail( "LZ4 compression failed" );

        outCompressed.SetItemCount( foundation::NumericCast< Count >( compressedSize ) );

        return BoolResult::Succeed();
    }

    BoolResult Decompress( const Byte *pData,
        const Count compressedSize,
        const Count originalSize,
        primitive::Vector< Byte > &outDecompressed )
    {
        outDecompressed.SetItemCount( originalSize );

        const int result = LZ4_decompress_safe( reinterpret_cast< const char * >( pData ),
            reinterpret_cast< char * >( outDecompressed.GetData() ),
            foundation::NumericCast< int >( compressedSize ),
            foundation::NumericCast< int >( originalSize ) );

        if ( result < 0 )
            return BoolResult::Fail( "LZ4 decompression failed" );

        return BoolResult::Succeed();
    }
}
