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
 * @file        text_stream_tests.cpp
 * @author      Zenn Geeraerts
 * @created     2 December 2025
 * @brief		Text stream unit tests
 */
#include "smile/common/stream/text_stream.h"

#include <catch/catch.hpp>

namespace smile::stream
{
    TEST_CASE( "TextStream", "[stream]" )
    {
        SECTION( "getText" )
        {
            TextStream stream;

            stream << "Hello"
                   << " "
                   << "World!";

            CHECK( primitive::StringView{ "Hello World!" } == stream.GetText() );
        }

        SECTION( "Operators" )
        {
            TextStream stream1, stream2;

            stream1 << "Hello";
            stream2 << "World";

            stream1 << " " << stream2 << "!";

            CHECK( primitive::StringView{ "Hello World!" } == stream1.GetText() );
        }

        SECTION( "Rewind" )
        {
            char bigBuffer[1024];
            TextStream stream;

            std::memset( bigBuffer, 'A', sizeof( bigBuffer ) - 1 );
            bigBuffer[1023] = 0;

            stream.Reset();
            stream << "Hello Xorld";
            CHECK( 11 == stream.GetText().GetCharCount() );
            CHECK( primitive::StringView{ "Hello Xorld" } == stream.GetText() );

            stream.Rewind( 6 );
            CHECK( 6 == stream.GetText().GetCharCount() );
            CHECK( primitive::StringView{ "Hello " } == stream.GetText() );

            stream << "World";
            CHECK( primitive::StringView{ "Hello World" } == stream.GetText() );

            CHECK_NOASSERT( stream.Rewind( 11 ) );
            CHECK_ASSERT( stream.Rewind( 12 ) );

            stream.Reset();
            stream << bigBuffer << "Hello World";
            stream.Rewind( 1020 );
            CHECK( 1020 == stream.GetText().GetCharCount() );
            CHECK( 'A' == stream.GetText()[1019] );

            stream.Reset();
            stream << bigBuffer << "HelloWorld";
            stream.Rewind( 1028 );
            CHECK( 1028 == stream.GetText().GetCharCount() );
            stream << " World";
            CHECK( 'A' == stream.GetText()[1022] );
            CHECK( primitive::StringView{ "Hello World" } ==
                   primitive::StringView{ stream.GetText().GetSubText() + 1023 } );
        }

        SECTION( "Write" )
        {
            TextStream stream;

            stream.Write( "Hello", 5 );
            stream.Write( " World!", 7 );

            CHECK( primitive::StringView{ "Hello World!" } == stream.GetText() );
        }
    }
}