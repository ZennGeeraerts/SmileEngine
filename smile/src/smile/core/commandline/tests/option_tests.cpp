/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/core/commandline/parser.h"
#include "smile/core/commandline/option.h"
#include "smile/core/commandline/arguments.h"

#include <catch/catch.hpp>

namespace smile::commandline
{
    TEST_CASE( "Option", "[commandline]" )
    {
        SECTION( "integer-parser" )
        {
            const char *optionName{ "integer-parser" };
            Option< int > parser{ optionName, Description{ "Parse a string to an integer" } };
            const Count argCount{ 3 };

            const char *args[] = { "-", "--value", "10000" };
            Arguments arguments{ { args, argCount } };

            CHECK( parser.Parse( arguments.Pop() ) );
            CHECK( parser.Value == 10000 );

            const char *args2[] = { "-", "--value", "500" };
            Arguments arguments2{ { args2, argCount } };

            CHECK( parser.Parse( arguments2.Pop() ) );
            CHECK( parser.Value == 500 );

            const char *args3[] = { "-", "--value", "this is not an integer" };
            Arguments arguments3{ { args3, argCount } };

            CHECK( !parser.Parse( arguments3.Pop() ) );

            const char *args4[] = { "-", "--value", "365" };
            Arguments arguments4{ { args4, argCount } };

            CHECK( parser.Parse( arguments4.Pop() ) );
            CHECK( parser.Value == 365 );

            const char *args5[]{ "-", "--value", "0" };
            Arguments arguments5{ { args5, argCount } };

            CHECK( parser.Parse( arguments5.Pop() ) );
            CHECK( parser.Value == 0 );
        }

        SECTION( "string-parser" )
        {
            const char *optionName{ "string-parser" };
            Option< primitive::String > parser{ optionName, Description{ "Parse a string to a string" } };
            const Count argCount{ 3 };

            const char *args[] = { "-", "-t", "this is a test string" };
            Arguments arguments{ { args, argCount } };

            CHECK( parser.Parse( arguments.Pop() ) );
            CHECK( parser.Value == "this is a test string" );
        }
    }
}