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
 * @file        lz4_compression_tests.cpp
 * @author      Zenn Geeraerts
 * @created     9 May 2026
 * @brief       Unit tests for LZ4 compression and decompression
 */
#include "smile/core/compression/lz4/lz4_compression.h"

#include <catch/catch.hpp>

namespace smile::compression::lz4
{
    TEST_CASE( "LZ4 Compression" )
    {
        SECTION( "Compress succeeds on valid input", "[LZ4]" )
        {
            const char input[] = "SmileEngine LZ4 compression test data. Hello, World!";
            const Count size = static_cast< Count >( sizeof( input ) );

            primitive::Vector< Byte > compressed;
            const BoolResult result = Compress( reinterpret_cast< const Byte * >( input ), size, compressed );

            REQUIRE( result );
            REQUIRE( compressed.GetItemCount() > 0 );
        }

        SECTION( "Decompress round-trips data correctly", "[LZ4]" )
        {
            const char input[] = "SmileEngine LZ4 compression test data. Hello, World!";
            const Count originalSize = static_cast< Count >( sizeof( input ) );

            primitive::Vector< Byte > compressed;
            REQUIRE( Compress( reinterpret_cast< const Byte * >( input ), originalSize, compressed ) );

            primitive::Vector< Byte > decompressed;
            const BoolResult result =
                Decompress( compressed.GetData(), compressed.GetItemCount(), originalSize, decompressed );

            REQUIRE( result );
            REQUIRE( decompressed.GetItemCount() == originalSize );
            REQUIRE( memcmp( input, decompressed.GetData(), originalSize ) == 0 );
        }

        SECTION( "Compressed size is smaller than original for compressible data", "[LZ4]" )
        {
            // Highly repetitive data compresses well
            primitive::Vector< Byte > input( 1024, static_cast< Byte >( 'A' ) );

            primitive::Vector< Byte > compressed;
            REQUIRE( Compress( input.GetData(), input.GetItemCount(), compressed ) );

            REQUIRE( compressed.GetItemCount() < input.GetItemCount() );
        }

        SECTION( "Decompress fails on invalid compressed data", "[LZ4]" )
        {
            const Byte garbage[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            const Count fakeOriginalSize = 64;

            primitive::Vector< Byte > decompressed;

            const BoolResult result =
                Decompress( garbage, static_cast< Count >( sizeof( garbage ) ), fakeOriginalSize, decompressed );

            REQUIRE( !result );
        }
    }
}
