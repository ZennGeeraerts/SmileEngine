/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/primitive/collection/array.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "Array" )
    {
        SECTION( "Constructor" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

            for ( int i{}; i < 10; ++i )
            {
                REQUIRE( values[i] == i );
            }
        }

        SECTION( "Copy constructor" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            Array< int, 10 > other{ values };

            for ( int i{}; i < 10; ++i )
            {
                REQUIRE( other[i] == i );
            }
        }

        SECTION( "Set" )
        {
            Array< int, 10 > values;
            values.Set( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );

            for ( int i{}; i < 10; ++i )
            {
                REQUIRE( values[i] == i );
            }
        }

        SECTION( "operator=" )
        {
            Array< int, 10 > values;
            values = Array< int, 10 >{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

            for ( int i{}; i < 10; ++i )
            {
                REQUIRE( values[i] == i );
            }
        }

        SECTION( "IsValidIndex" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

            for ( int i{}; i < 10; ++i )
            {
                REQUIRE( values.IsValidIndex( i ) );
            }

            REQUIRE( !values.IsValidIndex( -1 ) );
            REQUIRE( !values.IsValidIndex( 10 ) );
        }

        SECTION( "GetItemAtIndex" )
        {
            Array< int, 2 > values{ 10, 20 };

            REQUIRE( 10 == values.GetItemAtIndex( 0 ) );
            REQUIRE( 20 == values.GetItemAtIndex( 1 ) );
        }

        SECTION( "SetItemAtIndex" )
        {
            Array< int, 2 > values{ 10, 20 };

            values.SetItemAtIndex( 30, 1 );

            REQUIRE( 10 == values.GetItemAtIndex( 0 ) );
            REQUIRE( 30 == values.GetItemAtIndex( 1 ) );
        }

        SECTION( "GetItemCount" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

            REQUIRE( 10 == values.GetItemCount() );
        }

        SECTION( "Range based for loop" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            int expected = 0;

            for ( int value : values )
            {
                REQUIRE( expected == value );
                ++expected;
            }
        }

        SECTION( "operator==" )
        {
            Array< int, 10 > values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            Array< int, 10 > other{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            Array< int, 10 > other2{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };

            REQUIRE( values == other );
            REQUIRE( values != other2 );
        }
    }
}