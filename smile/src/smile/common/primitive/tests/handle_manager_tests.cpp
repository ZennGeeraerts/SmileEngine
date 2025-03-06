/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "primitive/handle_manager.h"

#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "Handle Manager" )
    {
        SECTION( "CreateHandle" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            REQUIRE( handle.GetIndex() == 0 );
            REQUIRE( handle2.GetIndex() == 1 );
        }

        SECTION( "GetHandle" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            REQUIRE( handleManager.GetHandle( 0 ) == handle );
            REQUIRE( handleManager.GetHandle( 1 ) == handle2 );
        }

        SECTION( "GetHandleCount" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            REQUIRE( handleManager.GetHandleCount() == 2 );
        }

        SECTION( "DestroyHandle" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            handleManager.DestroyHandle( handle );

            REQUIRE( !handleManager.GetHandle( handle.GetIndex() ).IsValid() );
            REQUIRE( handleManager.GetHandle( handle2.GetIndex() ).IsValid() );
        }

        SECTION( "IsHandleActive" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            handleManager.DestroyHandle( handle2 );

            REQUIRE( handleManager.IsHandleActive( handle ) );
            REQUIRE( !handleManager.IsHandleActive( handle2 ) );
            REQUIRE( !handleManager.IsHandleActive( primitive::Handle{} ) );
        }

        SECTION( "Recycle handles" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            handleManager.DestroyHandle( handle2 );

            primitive::Handle handle3 = handleManager.CreateHandle();

            REQUIRE( handle.GetIndex() == 0 );
            REQUIRE( handle3.GetIndex() == 1 );
        }

        SECTION( "Iterate" )
        {
            primitive::HandleManager handleManager{};

            primitive::Handle handle = handleManager.CreateHandle();
            primitive::Handle handle2 = handleManager.CreateHandle();

            handleManager.DestroyHandle( handle2 );

            primitive::Handle handle3 = handleManager.CreateHandle();
            primitive::Handle handle4 = handleManager.CreateHandle();

            std::vector< primitive::Handle<> > expected{ handle, handle3, handle4 };
            std::vector< primitive::Handle<> > result{};

            for ( auto h : handleManager )
            {
                result.push_back( h );
            }

            REQUIRE( result == expected );
        }
    }
}