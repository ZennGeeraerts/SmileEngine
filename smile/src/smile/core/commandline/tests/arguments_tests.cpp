/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/core/commandline/arguments.h"
#include "smile/common/primitive/collection/array.h"

#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Arguments", "[commandline]" )
    {
        SECTION( "Pop with valid args" )
        {
            primitive::Array< const char *, 5 > array{ "-", "--test-arg", "sample text", "--another-arg", "more text" };
            commandline::Arguments args{ array.AsView() };

            auto arg1 = args.Pop();

            REQUIRE( arg1.IsValid() );
            REQUIRE( arg1.GetName() == "test-arg" );
            REQUIRE( arg1.GetValue() == "sample text" );

            auto arg2 = args.Pop();

            REQUIRE( arg2.IsValid() );
            REQUIRE( arg2.GetName() == "another-arg" );
            REQUIRE( arg2.GetValue() == "more text" );
        }

        SECTION( "Pop with invalid args" )
        {
            primitive::Array< const char *, 5 > array{ "-", "--valid", "sample text", "%%invalid", "more text" };

            commandline::Arguments args{ array.AsView() };

            REQUIRE_NOASSERT( args.Pop() );
            REQUIRE_ASSERT( args.Pop() );
        }

        SECTION( "IsFinished" )
        {
            primitive::Array< const char *, 5 > array{ "-", "--test-arg", "sample text", "--another-arg", "more text" };
            commandline::Arguments args{ array.AsView() };

            REQUIRE_FALSE( args.IsFinished() );

            args.Pop();
            REQUIRE_FALSE( args.IsFinished() );

            args.Pop();
            REQUIRE( args.IsFinished() );
        }

        SECTION( "HasArgument returns false with invalid argument" )
        {
            primitive::Array< const char *, 5 > array{ "-", "--valid", "sample text", "%%invalid", "more text" };
            commandline::Arguments args{ array.AsView() };

            REQUIRE( args.HasArgument() );

            args.Pop();
            REQUIRE_FALSE( args.HasArgument() );
        }

        SECTION( "HasArgument returns false when finished" )
        {
            primitive::Array< const char *, 5 > array{ "-", "--test-arg", "sample text", "--another-arg", "more text" };
            commandline::Arguments args{ array.AsView() };

            REQUIRE( args.HasArgument() );

            args.Pop();
            REQUIRE( args.HasArgument() );

            args.Pop();
            REQUIRE_FALSE( args.HasArgument() );
        }
    }
}