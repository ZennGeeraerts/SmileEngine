/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        name_tests.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Name unit tests
 */
#include "smile/common/primitive/name.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "Primitive Name", "[primitive]" )
    {
        SECTION( "Names are unique" )
        {
            Name test{ "test" };
            Name testString{ primitive::String{ "test" } };
            Name testStringView{ primitive::StringView{ "test" } };

            CHECK( !test.IsEmpty() );
            CHECK( !testString.IsEmpty() );
            CHECK( !testStringView.IsEmpty() );
            CHECK( test == testString );
            CHECK( test == testStringView );
            CHECK( testString == test );
            CHECK( testString == testStringView );
            CHECK( testStringView == test );
            CHECK( testStringView == testString );
        }

        SECTION( "Clear" )
        {
            primitive::Name test{ "test" };

            CHECK( !test.IsEmpty() );

            test.Clear();

            CHECK( test.IsEmpty() );

            test = primitive::Name{ "test2" };

            CHECK( !test.IsEmpty() );

            test.Clear();

            CHECK( test.IsEmpty() );
        }
    }
}