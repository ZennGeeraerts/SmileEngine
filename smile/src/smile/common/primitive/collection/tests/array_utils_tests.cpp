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
            REQUIRE_FALSE( array::FindItemIndex( rawItems, 8 ) );
            REQUIRE( array::FindItemIndex( rawItems, 9 ) );
            REQUIRE( array::FindItemIndex( rawItems, 9 ) == 2 );
        }

        SECTION( "IsEqual" )
        {
            Vector< int > a{ 1, 2, 4, 5 };
            Vector< int > b{ 1, 2, 4 };
            Vector< int > c{ 1, 2, 4, 6 };
            Vector< int > d{ 1, 2, 4, 5, 6 };
            Vector< int > e{ 1, 2, 4, 5 };
            Vector< int > empty1{};
            Vector< int > empty2{};

            REQUIRE( array::IsEqual( empty1, empty2 ) );
            REQUIRE_FALSE( array::IsEqual( empty1, a ) );
            REQUIRE_FALSE( array::IsEqual( a, b ) );
            REQUIRE_FALSE( array::IsEqual( a, c ) );
            REQUIRE_FALSE( array::IsEqual( a, d ) );
            REQUIRE( array::IsEqual( a, e ) );
            REQUIRE_FALSE( array::IsEqual( a, empty1 ) );
            REQUIRE_FALSE( array::IsEqual( b, a ) );
            REQUIRE_FALSE( array::IsEqual( c, a ) );
            REQUIRE_FALSE( array::IsEqual( d, a ) );
            REQUIRE( array::IsEqual( e, a ) );
        }
    }

    struct Value
    {
        int Val;
    };

    TEST_CASE( "Sorted search helpers", "[primitive][collection]" )
    {
        const int sortedArray[] = { 1, 3, 5, 7, 9 };
        const Value valueSortedArray[] = { 1, 3, 5, 7, 9 };
        auto predicate = []( const Value &first, const Value &second ) { return first.Val < second.Val; };

        SECTION( "GetLowerBoundIndex" )
        {
            CHECK( 0 == array::GetLowerBoundIndex( sortedArray, 0 ) );
            CHECK( 0 == array::GetLowerBoundIndex( sortedArray, 1 ) );
            CHECK( 1 == array::GetLowerBoundIndex( sortedArray, 2 ) );
            CHECK( 1 == array::GetLowerBoundIndex( sortedArray, 3 ) );
            CHECK( 2 == array::GetLowerBoundIndex( sortedArray, 4 ) );
            CHECK( 2 == array::GetLowerBoundIndex( sortedArray, 5 ) );
            CHECK( 3 == array::GetLowerBoundIndex( sortedArray, 6 ) );
            CHECK( 3 == array::GetLowerBoundIndex( sortedArray, 7 ) );
            CHECK( 4 == array::GetLowerBoundIndex( sortedArray, 8 ) );
            CHECK( 4 == array::GetLowerBoundIndex( sortedArray, 9 ) );
            CHECK( 5 == array::GetLowerBoundIndex( sortedArray, 10 ) );
        }

        SECTION( "GetLowerBoundIndex with predicate" )
        {
            CHECK( 0 == array::GetLowerBoundIndex( valueSortedArray, Value{ 0 }, predicate ) );
            CHECK( 0 == array::GetLowerBoundIndex( valueSortedArray, Value{ 1 }, predicate ) );
            CHECK( 1 == array::GetLowerBoundIndex( valueSortedArray, Value{ 2 }, predicate ) );
            CHECK( 1 == array::GetLowerBoundIndex( valueSortedArray, Value{ 3 }, predicate ) );
            CHECK( 2 == array::GetLowerBoundIndex( valueSortedArray, Value{ 4 }, predicate ) );
            CHECK( 2 == array::GetLowerBoundIndex( valueSortedArray, Value{ 5 }, predicate ) );
            CHECK( 3 == array::GetLowerBoundIndex( valueSortedArray, Value{ 6 }, predicate ) );
            CHECK( 3 == array::GetLowerBoundIndex( valueSortedArray, Value{ 7 }, predicate ) );
            CHECK( 4 == array::GetLowerBoundIndex( valueSortedArray, Value{ 8 }, predicate ) );
            CHECK( 4 == array::GetLowerBoundIndex( valueSortedArray, Value{ 9 }, predicate ) );
            CHECK( 5 == array::GetLowerBoundIndex( valueSortedArray, Value{ 10 }, predicate ) );
        }

        SECTION( "GetUpperBoundIndex" )
        {
            CHECK( 0 == array::GetUpperBoundIndex( sortedArray, 0 ) );
            CHECK( 1 == array::GetUpperBoundIndex( sortedArray, 1 ) );
            CHECK( 1 == array::GetUpperBoundIndex( sortedArray, 2 ) );
            CHECK( 2 == array::GetUpperBoundIndex( sortedArray, 3 ) );
            CHECK( 2 == array::GetUpperBoundIndex( sortedArray, 4 ) );
            CHECK( 3 == array::GetUpperBoundIndex( sortedArray, 5 ) );
            CHECK( 3 == array::GetUpperBoundIndex( sortedArray, 6 ) );
            CHECK( 4 == array::GetUpperBoundIndex( sortedArray, 7 ) );
            CHECK( 4 == array::GetUpperBoundIndex( sortedArray, 8 ) );
            CHECK( 5 == array::GetUpperBoundIndex( sortedArray, 9 ) );
            CHECK( 5 == array::GetUpperBoundIndex( sortedArray, 10 ) );
        }

        SECTION( "GetUpperBoundIndex with predicate" )
        {
            CHECK( 0 == array::GetUpperBoundIndex( valueSortedArray, Value{ 0 }, predicate ) );
            CHECK( 1 == array::GetUpperBoundIndex( valueSortedArray, Value{ 1 }, predicate ) );
            CHECK( 1 == array::GetUpperBoundIndex( valueSortedArray, Value{ 2 }, predicate ) );
            CHECK( 2 == array::GetUpperBoundIndex( valueSortedArray, Value{ 3 }, predicate ) );
            CHECK( 2 == array::GetUpperBoundIndex( valueSortedArray, Value{ 4 }, predicate ) );
            CHECK( 3 == array::GetUpperBoundIndex( valueSortedArray, Value{ 5 }, predicate ) );
            CHECK( 3 == array::GetUpperBoundIndex( valueSortedArray, Value{ 6 }, predicate ) );
            CHECK( 4 == array::GetUpperBoundIndex( valueSortedArray, Value{ 7 }, predicate ) );
            CHECK( 4 == array::GetUpperBoundIndex( valueSortedArray, Value{ 8 }, predicate ) );
            CHECK( 5 == array::GetUpperBoundIndex( valueSortedArray, Value{ 9 }, predicate ) );
            CHECK( 5 == array::GetUpperBoundIndex( valueSortedArray, Value{ 10 }, predicate ) );
        }
    }
}