/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/fixed_vector.h"

#include <catch/catch.hpp>

struct ComplexType final
{
    ComplexType() = default;
    ComplexType( int i, float f, const std::string &str ) : I{ i }, F{ f }, Str{ str }
    {
    }

    int I;
    float F;
    std::string Str;
};

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

        SECTION( "EmplaceBack" )
        {
            FixedVector< ComplexType, 10 > values{};

            values.EmplaceBack( 1, 1.0f, "1" );
            values.EmplaceBack( 2, 2.0f, "2" );

            REQUIRE( values[0].I == 1 );
            REQUIRE( values[1].I == 2 );
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

        SECTION( "GetItemCount" )
        {
            FixedVector< int, 10 > values{};

            REQUIRE( values.GetItemCount() == 0 );
            REQUIRE( values.GetMaxItemCount() == 10 );

            values.PushBack( 1 );
            values.PushBack( 2 );

            REQUIRE( values.GetItemCount() == 2 );
            REQUIRE( values.GetMaxItemCount() == 10 );
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

            REQUIRE( values.GetItemCount() == 5 );
            for ( auto value : values )
            {
                REQUIRE( expected == value );
                ++expected;
            }

            values.PopBack();
            expected = 0;

            REQUIRE( values.GetItemCount() == 4 );
            for ( auto value : values )
            {
                REQUIRE( expected == value );
                ++expected;
            }
        }
    }
}