/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/primitive/text/string.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "String", "[primitive][text]" )
    {
        SECTION( "constructor" )
        {
            String empty, reference{ "test" }, value{ reference }, fromStringView{ StringView{ "test2" } };

            CHECK( strcmp( empty.GetData(), "" ) == 0 );
            CHECK( strcmp( reference.GetData(), "test" ) == 0 );
            CHECK( strcmp( value.GetData(), "test" ) == 0 );
            CHECK( strcmp( fromStringView.GetData(), "test2" ) == 0 );
        }

        SECTION( "operator assignment" )
        {
            String value;

            value = "test";
            CHECK( strcmp( value.GetData(), "test" ) == 0 );

            value = StringView{ "test2" };
            CHECK( strcmp( value.GetData(), "test2" ) == 0 );
        }

        SECTION( "operator index" )
        {
            const String test{ "test" };
            String test2{ "test2" };

            CHECK( test[3] == 't' );
            CHECK( test2[4] == '2' );

            test2[1] = 'a';
            CHECK( !strcmp( test2.GetData(), "tast2" ) );

            REQUIRE_ASSERT( test[5] );
            REQUIRE_ASSERT( test[6] );
        }

        SECTION( "operator plus equal" )
        {
            String value, otherValue, someText;

            value += "test1";
            CHECK( strcmp( "test1", value.GetData() ) == 0 );

            value += "test2";
            CHECK( strcmp( "test1test2", value.GetData() ) == 0 );

            someText = "test3";

            otherValue += someText;
            CHECK( strcmp( "test3", otherValue.GetData() ) == 0 );

            otherValue += someText;
            CHECK( strcmp( "test3test3", otherValue.GetData() ) == 0 );
        }

        SECTION( "GetCharCount / IsEmpty" )
        {
            String empty, emptyStr{ "" }, test{ "test" };

            CHECK( empty.GetCharCount() == 0 );
            CHECK( emptyStr.GetCharCount() == 0 );
            CHECK( test.GetCharCount() == 4 );

            CHECK( empty.IsEmpty() );
            CHECK( emptyStr.IsEmpty() );
            CHECK( !test.IsEmpty() );
        }

        SECTION( "SetCharCount" )
        {
            String empty, test{ "test long" };

            empty.SetCharCount( 4 );
            CHECK( empty.GetCharCount() == 4 );

            test.SetCharCount( 6 );
            CHECK( test == "test l" );

            test.SetCharCount( 1 );
            CHECK( test == "t" );

            test.SetCharCount( 0 );
            CHECK( test.IsEmpty() );
        }

        SECTION( "operator plus" )
        {
            String result, test{ "test" }, test2{ "test2" };

            result = "some " + test;
            CHECK( strcmp( result.GetData(), "some test" ) == 0 );

            result = test + " some";
            CHECK( strcmp( result.GetData(), "test some" ) == 0 );

            result = test + test2;
            CHECK( strcmp( result.GetData(), "testtest2" ) == 0 );

            result = test + "test3";
            CHECK( strcmp( result.GetData(), "testtest3" ) == 0 );
        }

        SECTION( "EraseCharAtIndex" )
        {
            String test{ "test" };

            test.EraseCharAtIndex( 2 );
            CHECK( test == "tet" );

            test.EraseCharAtIndex( 0 );
            CHECK( test == "et" );

            test.EraseCharAtIndex( test.GetCharCount() - 1 );
            CHECK( test == "e" );
        }
    }
}