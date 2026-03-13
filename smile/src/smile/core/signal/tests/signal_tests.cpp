#include "smile/core/signal/signal.h"

#include <catch/catch.hpp>

namespace smile::signal
{
    struct SumCommand
    {
        int a, b;
    };

    struct SumResult
    {
        int Value;
    };

    SumResult Sum( const SumCommand &cmd )
    {
        return { cmd.a + cmd.b };
    }

    TEST_CASE( "Signal" )
    {
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
    }
}