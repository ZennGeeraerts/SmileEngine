/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "engine/common/primitive/collection/sparse_set.h"

#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "SparseSet" )
    {
        SECTION( "Constructor should add elements to the sparse set" )
        {
            primitive::SparseSet< Uint32 > sparseSet{ 15, 16, 17, 18 };

            REQUIRE( sparseSet.GetElement( 0 ) == 15 );
            REQUIRE( sparseSet.GetElement( 1 ) == 16 );
            REQUIRE( sparseSet.GetElement( 2 ) == 17 );
            REQUIRE( sparseSet.GetElement( 3 ) == 18 );
        }

        SECTION( "Insert should add element at next index" )
        {
            primitive::SparseSet< Uint32 > sparseSet{};

            sparseSet.Insert( 15 );
            sparseSet.Insert( 16 );
            sparseSet.Insert( 17 );
            sparseSet.Insert( 18 );

            REQUIRE( sparseSet.GetElement( 0 ) == 15 );
            REQUIRE( sparseSet.GetElement( 1 ) == 16 );
            REQUIRE( sparseSet.GetElement( 2 ) == 17 );
            REQUIRE( sparseSet.GetElement( 3 ) == 18 );
        }

        SECTION( "Erase should swap last element with erased element" )
        {
            primitive::SparseSet< Uint32 > sparseSet{ 15, 16, 17, 18 };

            sparseSet.Erase( 16 );

            REQUIRE( sparseSet.GetElement( 0 ) == 15 );
            REQUIRE( sparseSet.GetElement( 1 ) == 18 );
            REQUIRE( sparseSet.GetElement( 2 ) == 17 );
        }

        SECTION( "Sort should sort the elements based on the sort function" )
        {
            primitive::SparseSet< Uint32 > sparseSet{ 16, 17, 18, 15 };

            REQUIRE( sparseSet.GetElement( 0 ) == 16 );
            REQUIRE( sparseSet.GetElement( 1 ) == 17 );
            REQUIRE( sparseSet.GetElement( 2 ) == 18 );
            REQUIRE( sparseSet.GetElement( 3 ) == 15 );

            REQUIRE( sparseSet.GetIndex( 16 ) == 0 );
            REQUIRE( sparseSet.GetIndex( 17 ) == 1 );
            REQUIRE( sparseSet.GetIndex( 18 ) == 2 );
            REQUIRE( sparseSet.GetIndex( 15 ) == 3 );

            sparseSet.Sort( []( Uint32 left, Uint32 right ) { return left < right; } );

            REQUIRE( sparseSet.GetElement( 0 ) == 15 );
            REQUIRE( sparseSet.GetElement( 1 ) == 16 );
            REQUIRE( sparseSet.GetElement( 2 ) == 17 );
            REQUIRE( sparseSet.GetElement( 3 ) == 18 );

            REQUIRE( sparseSet.GetIndex( 15 ) == 0 );
            REQUIRE( sparseSet.GetIndex( 16 ) == 1 );
            REQUIRE( sparseSet.GetIndex( 17 ) == 2 );
            REQUIRE( sparseSet.GetIndex( 18 ) == 3 );
        }
    }
}