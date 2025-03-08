/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "primitive/handle.h"

#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Handle" )
    {
        SECTION( "GetIndex" )
        {
            primitive::Handle handle{ 0, 0 };
            primitive::Handle handle2{ 1, 0 };

            REQUIRE( handle.GetIndex() == 0 );
            REQUIRE( handle2.GetIndex() == 1 );
        }

        SECTION( "IsValid" )
        {
            primitive::Handle handle{};
            primitive::Handle handle2{ 0, 0 };

            REQUIRE( !handle.IsValid() );
            REQUIRE( handle2.IsValid() );
        }

        SECTION( "NullHandle" )
        {
            auto nullHandle = primitive::Handle< Uint32 >::NullHandle();

            REQUIRE( nullHandle.GetIndex() == primitive::Handle< Uint32 >::s_MaxIndex );
            REQUIRE( !nullHandle.IsValid() );
        }

        SECTION( "Hash" )
        {
            primitive::Handle handle{ 0, 0 };
            primitive::Handle handle2{ 1, 0 };
            primitive::Handle handle3{ 0, 1 };
            primitive::Handle handle4{ 0, 0 };

            REQUIRE( handle.Hash() != handle2.Hash() );
            REQUIRE( handle.Hash() != handle3.Hash() );
            REQUIRE( handle.Hash() == handle4.Hash() );
        }

        SECTION( "operator== and operator!=" )
        {
            primitive::Handle handle{ 0, 0 };
            primitive::Handle handle2{ 1, 0 };
            primitive::Handle handle3{ 0, 1 };
            primitive::Handle handle4{ 0, 0 };

            REQUIRE( handle != handle2 );
            REQUIRE( handle != handle3 );
            REQUIRE( handle == handle4 );
        }

        SECTION( "operator bool()" )
        {
            primitive::Handle handle{ 0, 0 };
            primitive::Handle handle2{};

            REQUIRE( handle );
            REQUIRE( !handle2 );
        }
    }
}