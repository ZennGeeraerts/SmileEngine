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
 * @file        char_stream_tests.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Char stream unit tests
 */
#include "smile/common/stream/char_stream.h"

#include <catch/catch.hpp>

namespace smile
{
    class DummyClass : public stream::CharStream
    {

      public:
        void Write( const char *text, const Count itemCount ) override
        {
            Text += primitive::StringView( text, itemCount );
        }

        void Rewind( const Index charIndex ) override
        {
            Text.SetCharCount( charIndex );
        }

        primitive::String Text;
    };
}

namespace smile::stream
{
    TEST_CASE( "CharStream", "[stream]" )
    {

        SECTION( "Stream operators" )
        {
            DummyClass stream;
            auto *pointer = reinterpret_cast< void * >( 0x1aabbccdd );

            stream << 166 << " " << 1.5f << " " << 166u << " ";
            stream.Flush();

            CHECK( "166 1.500000 166 " == stream.Text );

            stream.Reset();
            stream.Text.Clear();

            stream << pointer;
            stream.Flush();

            CHECK( "1aabbccdd" == stream.Text );
        }

        SECTION( "Rewind" )
        {
            DummyClass stream;

            stream << "Hello Xorld";
            stream.Flush();

            CHECK( 11 == stream.Text.GetCharCount() );
            CHECK( "Hello Xorld" == stream.Text );

            stream.Rewind( 6 );
            CHECK( 6 == stream.Text.GetCharCount() );
            CHECK( "Hello " == stream.Text );

            stream << "World";
            stream.Flush();
            CHECK( "Hello World" == stream.Text );
        }

        SECTION( "addString" )
        {
            DummyClass stream;

            stream << "Hello World";
            stream.Flush();

            CHECK( 11 == stream.Text.GetCharCount() );
            CHECK( "Hello World" == stream.Text );

            stream.AddString( " and others!" );
            stream.Flush();

            CHECK( "Hello World and others!" == stream.Text );
        }

        SECTION( "isEmpty and reset" )
        {
            DummyClass stream;

            stream << "Hello World";
            stream.Flush();

            CHECK( 11 == stream.Text.GetCharCount() );
            CHECK( "Hello World" == stream.Text );

            stream.Reset();
            CHECK( stream.IsEmpty() );
        }
    }
}