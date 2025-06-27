/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/vector.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "Vector", "[primitive][collection]" )
    {
        SECTION( "Vector constructors" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values[0] == 1 );
            REQUIRE( values[1] == 2 );
            REQUIRE( values[2] == 3 );
        }

        SECTION( "GetItemAtIndex" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetItemAtIndex( 0 ) == 1 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 2 );
            REQUIRE( values.GetItemAtIndex( 2 ) == 3 );
        }

        SECTION( "IsValidIndex" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.IsValidIndex( 0 ) );
            REQUIRE( values.IsValidIndex( 1 ) );
            REQUIRE( values.IsValidIndex( 2 ) );

            REQUIRE_FALSE( values.IsValidIndex( -1 ) );
            REQUIRE_FALSE( values.IsValidIndex( 3 ) );
        }

        SECTION( "PushBack" )
        {
            Vector< int > values{};

            values.PushBack( 1 );
            values.PushBack( 2 );
            values.PushBack( 3 );

            REQUIRE( values[0] == 1 );
            REQUIRE( values[1] == 2 );
            REQUIRE( values[2] == 3 );
        }

        SECTION( "PopBack" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.PopBack();

            REQUIRE( values.GetItemCount() == 2 );
            REQUIRE( values[0] == 1 );
            REQUIRE( values[1] == 2 );
        }

        SECTION( "GetLastItem" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetLastItem() == 3 );
        }

        SECTION( "GetLastIndex" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetLastIndex() == 2 );
        }

        SECTION( "GetFirstItem" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetFirstItem() == 1 );
        }

        SECTION( "IsEmpty" )
        {
            Vector< int > values{};

            REQUIRE( values.IsEmpty() );

            values.PushBack( 1 );

            REQUIRE_FALSE( values.IsEmpty() );
        }

        SECTION( "GetItemIndex" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetItemIndex( 1 ) == 0 );
            REQUIRE( values.GetItemIndex( 2 ) == 1 );
            REQUIRE( values.GetItemIndex( 3 ) == 2 );
        }

        SECTION( "Clear" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.Clear();

            REQUIRE( values.GetItemCount() == 0 );
            REQUIRE( values.GetData() == nullptr );
        }

        SECTION( "SetItemCount" )
        {
            Vector< int > values{ 1, 2, 3 };

            REQUIRE( values.GetItemCount() == 3 );

            values.SetItemCount( 2 );

            REQUIRE( values.GetItemCount() == 2 );

            values.SetItemCount( 4 );

            REQUIRE( values.GetItemCount() == 4 );
        }

        SECTION( "Insert" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.Insert( 1 );

            REQUIRE( values.GetItemCount() == 4 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 2 );
            REQUIRE( values.GetItemAtIndex( 2 ) == 2 );

            values.Insert( 10, 1 );

            REQUIRE( values.GetItemCount() == 5 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 10 );
        }

        SECTION( "PopFront" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.PopFront();

            REQUIRE( values.GetItemCount() == 2 );
            REQUIRE( values.GetItemAtIndex( 0 ) == 2 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 3 );
        }

        SECTION( "EraseAtIndex" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.EraseAtIndex( 1 );

            REQUIRE( values.GetItemCount() == 2 );
            REQUIRE( values.GetItemAtIndex( 0 ) == 1 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 3 );
        }

        SECTION( "Erase" )
        {
            Vector< int > values{ 1, 2, 3 };

            values.Erase( 2 );

            REQUIRE( values.GetItemCount() == 2 );
            REQUIRE( values.GetItemAtIndex( 0 ) == 1 );
            REQUIRE( values.GetItemAtIndex( 1 ) == 3 );
        }
    }
}