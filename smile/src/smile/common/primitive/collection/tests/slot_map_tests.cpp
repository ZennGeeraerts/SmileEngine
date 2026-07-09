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
 * @file        slot_map_tests.cpp
 * @author      Zenn Geeraerts
 * @created     9 July 2026
 * @brief       Tests for the SlotMap data structure.
 */
#include "smile/common/primitive/collection/slot_map.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    using SlotMapType = SlotMap< int, 16 >;

    TEST_CASE( "SlotMap", "[primitive]" )
    {
        SECTION( "SlotMap inserts and retrieves a value" )
        {
            SlotMapType map;

            auto handle = map.Insert( 42 );

            REQUIRE( map.HasItemAtSlot( handle ) );
            REQUIRE( map.GetItemAtSlot( handle ) == 42 );
        }

        SECTION( "SlotMap stores multiple values" )
        {
            SlotMapType map;

            auto h1 = map.Insert( 10 );
            auto h2 = map.Insert( 20 );
            auto h3 = map.Insert( 30 );

            REQUIRE( map.GetItemAtSlot( h1 ) == 10 );
            REQUIRE( map.GetItemAtSlot( h2 ) == 20 );
            REQUIRE( map.GetItemAtSlot( h3 ) == 30 );
        }

        SECTION( "SlotMap returns mutable references" )
        {
            SlotMapType map;

            auto handle = map.Insert( 5 );

            map.GetItemAtSlot( handle ) = 99;

            REQUIRE( map.GetItemAtSlot( handle ) == 99 );
        }

        SECTION( "SlotMap erase invalidates the handle" )
        {
            SlotMapType map;

            auto handle = map.Insert( 123 );

            REQUIRE( map.HasItemAtSlot( handle ) );

            map.EraseItemAtSlot( handle );

            REQUIRE_FALSE( map.HasItemAtSlot( handle ) );
        }

        SECTION( "SlotMap reuses freed slots" )
        {
            SlotMapType map;

            auto h1 = map.Insert( 1 );
            auto firstIndex = h1.GetIndex();

            map.EraseItemAtSlot( h1 );

            auto h2 = map.Insert( 2 );

            REQUIRE( h2.GetIndex() == firstIndex );

            REQUIRE( h2 != h1 );

            REQUIRE( map.GetItemAtSlot( h2 ) == 2 );
        }

        SECTION( "SlotMap old handle remains invalid after slot reuse" )
        {
            SlotMapType map;

            auto h1 = map.Insert( 100 );

            map.EraseItemAtSlot( h1 );

            auto h2 = map.Insert( 200 );

            REQUIRE_FALSE( map.HasItemAtSlot( h1 ) );
            REQUIRE( map.HasItemAtSlot( h2 ) );

            REQUIRE( map.GetItemAtSlot( h2 ) == 200 );
        }
    }
}