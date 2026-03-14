/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        signal_tests.cpp
 * @author      Zenn Geeraerts
 * @created     13 March 2026
 * @brief       Signal unit tests
 */
#include "smile/core/signal/signal.h"

#include <catch/catch.hpp>

namespace smile::signal
{
    struct SumCommand final
    {
        int a, b;
    };

    struct SumResult final
    {
        int Value;
    };

    SumResult Sum( const SumCommand &cmd )
    {
        return { cmd.a + cmd.b };
    }

    TEST_CASE( "Signal" )
    {
        SECTION( "Connect/Disconnect functions" )
        {
            SignalHandler< SumResult( const SumCommand & ) > signal;
            Sink sink{ signal };

            REQUIRE( signal.IsEmpty() );
            REQUIRE( sink.IsEmpty() );

            auto func = []( const SumCommand &cmd ) { return SumResult{ cmd.a + cmd.b }; };
            sink.Connect( func );

            REQUIRE( signal.GetItemCount() == 1 );
            REQUIRE( !sink.IsEmpty() );

            sink.Disconnect( func );

            REQUIRE( signal.IsEmpty() );
            REQUIRE( sink.IsEmpty() );
        }

        SECTION( "Publish executes calls" )
        {
            SignalHandler< void() > signal;
            Sink sink{ signal };

            bool published = false;
            auto func = [&]() { published = true; };
            sink.Connect( func );

            signal.Publish();

            REQUIRE( published );
        }

        SECTION( "Collect captures results from sinks" )
        {
            SignalHandler< SumResult( const SumCommand & ) > signal;
            Sink sink{ signal };

            sink.Connect( Sum );

            SumCommand cmd{ 3, 4 };
            primitive::Vector< SumResult > results{};
            signal.Collect( [&results]( const SumResult &result ) { results.PushBack( result ); }, cmd );

            REQUIRE( results[0].Value == 7 );

            sink.Disconnect( Sum );
        }

        SECTION( "Collect calls with no return value executes calls" )
        {
            SignalHandler< void() > signal;
            Sink sink{ signal };

            bool published = false;
            auto func = [&]() { published = true; };
            sink.Connect( func );

            signal.Collect( []() {} );

            REQUIRE( published );
        }
    }
}