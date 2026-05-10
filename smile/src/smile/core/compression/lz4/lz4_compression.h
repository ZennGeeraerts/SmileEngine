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
 * @file        lz4_compression.h
 * @author      Zenn Geeraerts
 * @created     9 May 2026
 * @brief       LZ4 compression and decompression utilities
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"

namespace smile::compression::lz4
{
    BoolResult Compress( const Byte *pData, const Count size, primitive::Vector< Byte > &outCompressed );

    BoolResult Decompress( const Byte *pData,
        const Count compressedSize,
        const Count originalSize,
        primitive::Vector< Byte > &outDecompressed );
}
