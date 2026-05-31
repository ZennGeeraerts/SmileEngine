/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/text/utils.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "primitive::String utilities", "[primitive][text]" )
    {
        SECTION( "Split" )
        {
            Vector< String > segments;
            Vector< StringView > subtextSegments;

            Split( segments, "Test Hello  234   Ok", ' ' );
            Split( subtextSegments, "Test Hello  234   Ok  ", ' ' );

            CHECK( 4 == segments.GetItemCount() );
            CHECK( 4 == subtextSegments.GetItemCount() );

            CHECK( "Test" == segments[0] );
            CHECK( "Hello" == segments[1] );
            CHECK( "234" == segments[2] );
            CHECK( "Ok" == segments[3] );

            CHECK( "Test" == subtextSegments[0] );
            CHECK( "Hello" == subtextSegments[1] );
            CHECK( "234" == subtextSegments[2] );
            CHECK( "Ok" == subtextSegments[3] );
        }

        SECTION( "ToInt" )
        {
            String _5{ "5" };
            String _55{ "55" };
            String _555{ "555" };
            String wrong{ "555ah" };
            String wrong2{ "ah" };
            String floating{ "55.5" };

            auto value = ToInt( _5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5 );

            value = ToInt( _55 );
            CHECK( value.has_value() );
            CHECK( value.value() == 55 );

            value = ToInt( _555 );
            CHECK( value.has_value() );
            CHECK( value.value() == 555 );

            value = ToInt( wrong );
            CHECK( !value.has_value() );

            value = ToInt( wrong2 );
            CHECK( !value.has_value() );

            value = ToInt( floating );
            CHECK( !value.has_value() );
        }

        SECTION( "ToFloat" )
        {
            String _5_5{ "5.5" };
            String __5{ ".5" };
            String _5{ "5" };
            String _0_5{ "0.5" };
            String _55_55{ "55.55" };

            auto value = ToFloat( _5_5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5.5f );

            value = ToFloat( __5 );
            CHECK( value.has_value() );
            CHECK( value.value() == .5f );

            value = ToFloat( _5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 5.f );

            value = ToFloat( _0_5 );
            CHECK( value.has_value() );
            CHECK( value.value() == 0.5f );

            value = ToFloat( _55_55 );
            CHECK( value.has_value() );
            CHECK( value.value() == 55.55f );
        }

        SECTION( "FindText" )
        {
            String text{ "This contains some text" };

            CHECK( FindText( text, "This" ) == 0 );
            CHECK( FindText( text, "text" ) == 19 );
            CHECK( FindText( text, "contains some" ) == 5 );

            CHECK( FindText( text, "this" ) == s_InvalidIndex );
            CHECK( FindText( text, "Text" ) == s_InvalidIndex );
            CHECK( FindText( text, "Contains Some" ) == s_InvalidIndex );

            String text2{ "This contains some text, precious text and unused text stuff" };

            CHECK( FindText( text2, "text", 5 ) == 19 );
            CHECK( FindText( text2, "text", 20 ) == 34 );
            CHECK( FindText( text2, "text", 35 ) == 50 );

            CHECK( FindText( text2, "This", 1 ) == s_InvalidIndex );
            CHECK( FindText( text2, "text", 51 ) == s_InvalidIndex );
        }

        SECTION( "FindCharacter" )
        {
            String text{ "primitive::Text_" };

            CHECK( 9 == FindCharacter( text, ':' ) );
            CHECK( 0 == FindCharacter( text, 'p' ) );
            CHECK( 15 == FindCharacter( text, '_' ) );

            CHECK( s_InvalidIndex == FindCharacter( text, 'y' ) );
        }

        SECTION( "ReplaceText" )
        {
            String reference{ "This is some text" };
            String result;

            result = reference;
            ReplaceText( result, "This is", "That's" );
            CHECK( result == "That's some text" );

            result = reference;
            ReplaceText( result, "text", "text." );
            CHECK( result == "This is some text." );

            result = reference;
            ReplaceText( result, "texts", "anything" );
            CHECK( result == reference );

            result = "Some text text twice";
            ReplaceText( result, "text", "both" );
            CHECK( result == "Some both both twice" );

            result = reference;
            ReplaceText( result, "some ", "" );
            CHECK( result == "This is text" );

            result = reference;
            CHECK_ASSERT( ReplaceText( result, "", "text" ) );
        }

        SECTION( "ReplaceTextInsideRange" )
        {
            String reference{ "This is some text" };
            String result;

            result = reference;
            ReplaceTextInsideRange( result, 0, 7, "That's" );
            CHECK( result == "That's some text" );

            result = reference;
            ReplaceTextInsideRange( result, 13, 4, "text." );
            CHECK( result == "This is some text." );

            result = reference;
            ReplaceTextInsideRange( result, 10, 0, "" );
            CHECK( result == reference );

            result = reference;
            ReplaceTextInsideRange( result, 0, 0, "Insert:" );
            CHECK( result == "Insert:This is some text" );

            result = reference;
            ReplaceTextInsideRange( result, 4, 0, ":insert:" );
            CHECK( result == "This:insert: is some text" );

            result = reference;
            ReplaceTextInsideRange( result, 17, 0, ":insert:" );
            CHECK( result == "This is some text:insert:" );

            result = reference;
            CHECK_ASSERT( ReplaceTextInsideRange( result, -1, 0, "text" ) );
            CHECK_ASSERT( ReplaceTextInsideRange( result, 0, 18, "text" ) );
            CHECK_ASSERT( ReplaceTextInsideRange( result, 17, 1, "text" ) );
        }

        SECTION( "StartsWith" )
        {
            const String text{ "Hello world!" };
            const String emptyText{ "" };
            const StringView validPrefixWithGarbage{ "HelloStupid", 5 };
            const StringView InvalidPrefixWithGarbage{ "HelloStupid", 6 };

            CHECK( StartsWith( text, "Hello" ) );
            CHECK( !StartsWith( emptyText, "Hello" ) );
            CHECK( StartsWith( text, validPrefixWithGarbage ) );
            CHECK( !StartsWith( text, InvalidPrefixWithGarbage ) );
            CHECK_ASSERT( StartsWith( "Anytext", emptyText ) );
        }

        SECTION( "EndsWith" )
        {
            const String text{ "Hello world!" };
            const String emptyText{ "" };
            const StringView validSuffixWithGarbage{ "world!", 6 };
            const StringView invalidSuffixWithGarbage( "aworld!", 7 );

            CHECK( EndsWith( text, "world!" ) );
            CHECK( !EndsWith( emptyText, "Hello" ) );
            CHECK( EndsWith( text, validSuffixWithGarbage ) );
            CHECK( !EndsWith( text, invalidSuffixWithGarbage ) );
            CHECK_ASSERT( EndsWith( "Anytext", emptyText ) );
        }

        SECTION( "ContainsText" )
        {
            const String text{ "This contains some text" };

            CHECK( ContainsText( text, "This" ) );
            CHECK( ContainsText( text, "text" ) );
            CHECK( ContainsText( text, "contains some" ) );

            CHECK( !ContainsText( text, "this" ) );
            CHECK( !ContainsText( text, "Text" ) );
            CHECK( !ContainsText( text, "Contains Some" ) );
        }

        SECTION( "GetSubText" )
        {
            String reference{ "This is some text" };

            String substringToCount = GetSubText( reference, 0, 4 );
            CHECK( substringToCount == "This" );

            String substringFromOffsetToEnd = GetSubText( reference, 6 );
            CHECK( substringFromOffsetToEnd == "s some text" );

            String substringFromOffsetToCount = GetSubText( reference, 4, 6 );
            CHECK( substringFromOffsetToCount == " is so" );

            reference = "Some text to test";

            CHECK( "Some " == GetSubText( reference, 0, 5 ) );
            CHECK( "text " == GetSubText( reference, 5, 5 ) );
            CHECK( "t t" == GetSubText( reference, 8, 3 ) );

            CHECK_ASSERT( GetSubText( reference, 8, 35 ) );
            CHECK_ASSERT( GetSubText( reference, 0, 18 ) );
            CHECK_ASSERT( GetSubText( reference, 17, 1 ) );
            CHECK_ASSERT( GetSubText( reference, -2, 35 ) );
        }

        SECTION( "GetStartingText" )
        {
            primitive::String text{ "This contains some text" };

            CHECK( GetStartingText( text, 4 ) == "This" );
            CHECK( GetStartingText( text, 13 ) == "This contains" );
        }

        SECTION( "GetEndingText" )
        {
            primitive::String text{ "This contains some text" };

            CHECK( GetEndingText( text, 4 ) == "text" );
            CHECK( GetEndingText( text, 9 ) == "some text" );
        }

        SECTION( "GetEndingTextAtIndex" )
        {
            primitive::String text{ "This contains some text" };

            CHECK( GetEndingTextAtIndex( text, 0 ) == "This contains some text" );
            CHECK( GetEndingTextAtIndex( text, 5 ) == "contains some text" );
            CHECK( GetEndingTextAtIndex( text, 19 ) == "text" );
        }
    }
}