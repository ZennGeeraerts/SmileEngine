/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/foundation/numeric_cast.h"

#include <catch/catch.hpp>

namespace smile::foundation
{
    TEST_CASE( "Numeric cast", "[foundation]" )
    {
        SECTION( "Cast UInt to UInt" )
        {
            REQUIRE( 255 == NumericCast< Uint8 >( Uint64( 255 ) ) );
            REQUIRE( 255 == NumericCast< Uint8 >( Uint32( 255 ) ) );
            REQUIRE( 255 == NumericCast< Uint8 >( Uint16( 255 ) ) );
            REQUIRE( 255 == NumericCast< Uint8 >( Uint8( 255 ) ) );

            REQUIRE( 65535 == NumericCast< Uint16 >( Uint64( 65535 ) ) );
            REQUIRE( 65535 == NumericCast< Uint16 >( Uint32( 65535 ) ) );
            REQUIRE( 65535 == NumericCast< Uint16 >( Uint16( 65535 ) ) );

            REQUIRE( 4294967295u == NumericCast< Uint32 >( Uint64( 4294967295u ) ) );
            REQUIRE( 4294967295u == NumericCast< Uint32 >( Uint32( 4294967295u ) ) );

            REQUIRE_ASSERT( NumericCast< Uint8 >( Uint16( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Uint32( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Uint64( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Uint32( std::numeric_limits< Uint16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Uint64( std::numeric_limits< Uint16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint32 >( Uint64( std::numeric_limits< Uint32 >::max() ) + 1 ) );
        }

        SECTION( "Cast UInt to Int" )
        {
            REQUIRE( 127 == NumericCast< Int8 >( Uint64( 127 ) ) );
            REQUIRE( 127 == NumericCast< Int8 >( Uint32( 127 ) ) );
            REQUIRE( 127 == NumericCast< Int8 >( Uint16( 127 ) ) );
            REQUIRE( 127 == NumericCast< Int8 >( Uint8( 127 ) ) );

            REQUIRE( 32767 == NumericCast< Int16 >( Uint64( 32767 ) ) );
            REQUIRE( 32767 == NumericCast< Int16 >( Uint32( 32767 ) ) );
            REQUIRE( 32767 == NumericCast< Int16 >( Uint16( 32767 ) ) );

            REQUIRE( 2147483647 == NumericCast< Int32 >( Uint64( 2147483647 ) ) );
            REQUIRE( 2147483647 == NumericCast< Int32 >( Uint32( 2147483647 ) ) );

            REQUIRE_ASSERT( NumericCast< Int8 >( Uint16( std::numeric_limits< Int8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Int8 >( Uint32( std::numeric_limits< Int8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Int8 >( Uint64( std::numeric_limits< Int8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Int16 >( Uint32( std::numeric_limits< Int16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Int16 >( Uint64( std::numeric_limits< Int16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Int32 >( Uint64( std::numeric_limits< Int32 >::max() ) + 1 ) );
        }

        SECTION( "Cast Int to UInt" )
        {
            REQUIRE( 255 == NumericCast< Uint8 >( Int64( 255 ) ) );
            REQUIRE( 255 == NumericCast< Uint8 >( Int32( 255 ) ) );
            REQUIRE( 255 == NumericCast< Uint8 >( Int16( 255 ) ) );
            REQUIRE( 127 == NumericCast< Uint8 >( Int8( 127 ) ) );

            REQUIRE( 65535 == NumericCast< Uint16 >( Int64( 65535 ) ) );
            REQUIRE( 65535 == NumericCast< Uint16 >( Int32( 65535 ) ) );
            REQUIRE( 32767 == NumericCast< Uint16 >( Int16( 32767 ) ) );

            REQUIRE( 4294967295u == NumericCast< Uint32 >( Int64( 4294967295u ) ) );
            REQUIRE( 2147483647u == NumericCast< Uint32 >( Int32( 2147483647 ) ) );

            REQUIRE_ASSERT( NumericCast< Uint8 >( Int16( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Int32( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Int64( std::numeric_limits< Uint8 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Int32( std::numeric_limits< Uint16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Int64( std::numeric_limits< Uint16 >::max() + 1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint32 >( Int64( std::numeric_limits< Uint32 >::max() ) + 1 ) );

            REQUIRE_ASSERT( NumericCast< Uint8 >( Int8( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Int16( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Int32( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint8 >( Int64( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Int16( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Int32( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint16 >( Int64( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint32 >( Int32( -1 ) ) );
            REQUIRE_ASSERT( NumericCast< Uint32 >( Int64( -1 ) ) );
        }
    }
}