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
 * @file        sort_key_tests.cpp
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Tests for 64-bit pass sort-key encoders
 */
#include "smile/graphic/renderer/draw/sort_key.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    TEST_CASE( "SortKey - Opaque", "[graphic][draw]" )
    {
        SECTION( "Different PSO dominates over depth" )
        {
            const SortKey lowPsoFarDepth = sort_key::EncodeOpaque( 1, 0, 1000.0f );
            const SortKey highPsoNearDepth = sort_key::EncodeOpaque( 2, 0, 0.1f );

            REQUIRE( lowPsoFarDepth < highPsoNearDepth );
        }

        SECTION( "Same PSO: material dominates over depth" )
        {
            const SortKey lowMatFar = sort_key::EncodeOpaque( 5, 1, 999.0f );
            const SortKey highMatNear = sort_key::EncodeOpaque( 5, 2, 0.5f );

            REQUIRE( lowMatFar < highMatNear );
        }

        SECTION( "Same PSO + material: nearer depth sorts first (front-to-back)" )
        {
            const SortKey near = sort_key::EncodeOpaque( 1, 1, 1.0f );
            const SortKey far = sort_key::EncodeOpaque( 1, 1, 50.0f );

            REQUIRE( near < far );
        }

        SECTION( "Identical inputs produce identical keys" )
        {
            const SortKey a = sort_key::EncodeOpaque( 7, 3, 12.5f );
            const SortKey b = sort_key::EncodeOpaque( 7, 3, 12.5f );

            REQUIRE( a == b );
        }
    }

    TEST_CASE( "SortKey - Transparent", "[graphic][draw]" )
    {
        SECTION( "Farther depth sorts first (back-to-front)" )
        {
            const SortKey far = sort_key::EncodeTransparent( 1, 1, 100.0f );
            const SortKey near = sort_key::EncodeTransparent( 1, 1, 1.0f );

            REQUIRE( far < near );
        }

        SECTION( "Depth dominates over PSO" )
        {
            const SortKey farLowPso = sort_key::EncodeTransparent( 1, 0, 50.0f );
            const SortKey nearHighPso = sort_key::EncodeTransparent( 99, 0, 1.0f );

            REQUIRE( farLowPso < nearHighPso );
        }

        SECTION( "Same depth: PSO sorts before material" )
        {
            const SortKey lowPsoHighMat = sort_key::EncodeTransparent( 1, 99, 10.0f );
            const SortKey highPsoLowMat = sort_key::EncodeTransparent( 2, 0, 10.0f );

            REQUIRE( lowPsoHighMat < highPsoLowMat );
        }
    }

    TEST_CASE( "SortKey - Shadow", "[graphic][draw]" )
    {
        SECTION( "PSO sorts first, material second" )
        {
            const SortKey a = sort_key::EncodeShadow( 1, 99 );
            const SortKey b = sort_key::EncodeShadow( 2, 0 );

            REQUIRE( a < b );
        }

        SECTION( "Same PSO: material orders" )
        {
            const SortKey lowMat = sort_key::EncodeShadow( 5, 1 );
            const SortKey highMat = sort_key::EncodeShadow( 5, 9 );

            REQUIRE( lowMat < highMat );
        }
    }

    TEST_CASE( "SortKey - Depth encoding monotonicity", "[graphic][draw]" )
    {
        SECTION( "Encoded depth preserves ordering for positive floats" )
        {
            REQUIRE( sort_key::EncodeDepth( 0.0f ) < sort_key::EncodeDepth( 0.1f ) );
            REQUIRE( sort_key::EncodeDepth( 0.1f ) < sort_key::EncodeDepth( 1.0f ) );
            REQUIRE( sort_key::EncodeDepth( 1.0f ) < sort_key::EncodeDepth( 1000.0f ) );
        }
    }
}
