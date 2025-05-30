/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/fixed_vector.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "FixedVector" )
    {
        SECTION( "PushBack" )
        {
            FixedVector< int, 10 > values{};

            values.PushBack( 1 );
            values.PushBack( 5 );

            REQUIRE( values[0] == 1 );
            REQUIRE( values[1] == 5 );
        }

        SECTION( "IsValidIndex" )
        {
            FixedVector< int, 10 > values{};

            REQUIRE( !values.IsValidIndex( 0 ) );

            values.PushBack( 10 );
            values.PushBack( 20 );

            REQUIRE( !values.IsValidIndex( -1 ) );
            REQUIRE( values.IsValidIndex( 0 ) );
            REQUIRE( values.IsValidIndex( 1 ) );
            REQUIRE( !values.IsValidIndex( 2 ) );
        }

        SECTION( "GetCurrentItemCount" )
        {
            FixedVector< int, 10 > values{};

            REQUIRE( values.GetCurrentItemCount() == 0 );

            values.PushBack( 1 );
            values.PushBack( 2 );

            REQUIRE( values.GetCurrentItemCount() == 2 );
        }

        SECTION( "IsEmpty" )
        {
            FixedVector< int, 10 > values{};

            REQUIRE( values.IsEmpty() );

            values.PushBack( 1 );

            REQUIRE( !values.IsEmpty() );
        }

        SECTION( "PopBack" )
        {
            FixedVector< int, 10 > values{ 0, 1, 2, 3, 4 };
            int expected = 0;

            REQUIRE( values.GetCurrentItemCount() == 5 );
            for ( auto value : values )
            {
                REQUIRE( expected == value );
                ++expected;
            }

            values.PopBack();
            expected = 0;

            REQUIRE( values.GetCurrentItemCount() == 4 );
            for ( auto value : values )
            {
                REQUIRE( expected == value );
                ++expected;
            }
        }
    }
}