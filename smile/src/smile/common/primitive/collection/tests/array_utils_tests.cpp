/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/primitive/collection/array_utils.h"
#include "smile/common/primitive/collection/vector.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "Array utils", "[primitive][collection]" )
    {
        SECTION( "HasItem" )
        {
            Vector< int > items{ 1, 2, 4, 5 };
            int rawItems[]{ 6, 7, 9 };

            REQUIRE( array::HasItem( items, 1 ) );
            REQUIRE( array::HasItem( items, 2 ) );
            REQUIRE_FALSE( array::HasItem( items, 3 ) );
            REQUIRE( array::HasItem( items, 4 ) );
            REQUIRE( array::HasItem( items, 5 ) );

            REQUIRE( array::HasItem( rawItems, 6 ) );
            REQUIRE( array::HasItem( rawItems, 7 ) );
            REQUIRE_FALSE( array::HasItem( rawItems, 8 ) );
            REQUIRE( array::HasItem( rawItems, 9 ) );
        }

        SECTION( "FindItemIndex" )
        {
            Vector< int > items{ 1, 2, 4, 5 };
            int rawItems[]{ 6, 7, 9 };

            REQUIRE( array::FindItemIndex( items, 1 ) );
            REQUIRE( array::FindItemIndex( items, 1 ) == 0 );
            REQUIRE( array::FindItemIndex( items, 2 ) );
            REQUIRE( array::FindItemIndex( items, 2 ) == 1 );
            REQUIRE_FALSE( array::FindItemIndex( items, 3 ) );
            REQUIRE( array::FindItemIndex( items, 4 ) );
            REQUIRE( array::FindItemIndex( items, 4 ) == 2 );
            REQUIRE( array::FindItemIndex( items, 5 ) );
            REQUIRE( array::FindItemIndex( items, 5 ) == 3 );
            REQUIRE( array::FindItemIndex( rawItems, 6 ) );
            REQUIRE( array::FindItemIndex( rawItems, 6 ) == 0 );
            REQUIRE( array::FindItemIndex( rawItems, 7 ) );
            REQUIRE( array::FindItemIndex( rawItems, 7 ) == 1 );
            REQUIRE( !array::FindItemIndex( rawItems, 8 ) );
            REQUIRE( array::FindItemIndex( rawItems, 9 ) );
            REQUIRE( array::FindItemIndex( rawItems, 9 ) == 2 );
        }
    }
}