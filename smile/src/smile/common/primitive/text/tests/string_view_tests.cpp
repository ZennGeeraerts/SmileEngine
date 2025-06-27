/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/primitive/text/string_view.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "StringView" )
    {
        SECTION( "Equality with null-terminated" )
        {
            StringView empty, hello{ "hello" }, helloWorld{ "hello world" }, world{ "world" };

            CHECK( empty == empty );
            CHECK_FALSE( empty == hello );
            CHECK_FALSE( empty == world );
            CHECK( hello == hello );
            CHECK_FALSE( hello == empty );
            CHECK_FALSE( hello == helloWorld );
            CHECK( helloWorld == helloWorld );
            CHECK_FALSE( helloWorld == empty );
            CHECK_FALSE( helloWorld == hello );
        }

        SECTION( "Equality with non null-terminated" )
        {
            StringView empty, hello{ "hello world", 5 }, helloWorld{ "hello world12345", 11 }, world{ "worldhello", 5 };

            CHECK( empty == empty );
            CHECK_FALSE( empty == hello );
            CHECK_FALSE( empty == world );
            CHECK( hello == hello );
            CHECK_FALSE( hello == empty );
            CHECK_FALSE( hello == helloWorld );
            CHECK( helloWorld == helloWorld );
            CHECK_FALSE( helloWorld == empty );
            CHECK_FALSE( helloWorld == hello );
        }

        SECTION( "Comparison with null-terminated" )
        {
            StringView empty, hello{ "hello" }, helloWorld{ "hello world" }, world{ "world" };

            CHECK( empty < hello );
            CHECK( empty < world );
            CHECK_FALSE( empty < empty );
            CHECK_FALSE( hello < hello );
            CHECK( hello < world );
            CHECK( hello < helloWorld );
            CHECK_FALSE( helloWorld < hello );
            CHECK_FALSE( world < hello );
        }

        SECTION( "Comparison with non null-terminated" )
        {
            StringView empty, hello{ "helloworld", 5 }, helloWorld( "hello world12345", 11 ), world{ "worldhello", 5 };

            CHECK( empty < hello );
            CHECK( empty < world );
            CHECK_FALSE( empty < empty );
            CHECK_FALSE( hello < hello );
            CHECK( hello < world );
            CHECK( hello < helloWorld );
            CHECK_FALSE( helloWorld < hello );
            CHECK_FALSE( world < hello );
        }

        SECTION( "Comparison with prefix" )
        {
            StringView hello{ "helloworld", 5 }, helloWorld{ "helloworld", 10 };

            CHECK( hello < helloWorld );
            CHECK_FALSE( helloWorld < hello );
        }
    }
}