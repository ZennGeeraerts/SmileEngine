#include <smile_engine.h>
#define CATCH_CONFIG_RUNNER
#include <thirdparty/catch.hpp>

int main( int argc, char *argv[] )
{
    return Catch::Session().run( argc, argv );
}