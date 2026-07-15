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
 * @file        render_resource_table_tests.cpp
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Tests for the generation-checked, ref-counted render resource table
 */
#include "smile/graphic/renderer/scene/render_resource_table.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    namespace
    {
        struct Sample final
        {
            Uint32 Value = 0;
        };
    }

    TEST_CASE( "RenderResourceTable", "[graphic][scene]" )
    {
        SECTION( "Insert returns a valid alive handle with initial ref-count of one" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle h = table.Insert( Sample{ 42u } );

            REQUIRE( h.IsValid() );
            REQUIRE( table.IsAlive( h ) );
            REQUIRE( table.GetRefCount( h ) == 1u );
            REQUIRE( table.Get( h ).Value == 42u );
            REQUIRE( table.GetAliveCount() == 1 );
        }

        SECTION( "Release at refcount zero frees the slot" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle h = table.Insert( Sample{ 1u } );

            const bool freed = table.Release( h );
            REQUIRE( freed );
            REQUIRE_FALSE( table.IsAlive( h ) );
            REQUIRE( table.TryGet( h ) == nullptr );
            REQUIRE( table.GetAliveCount() == 0 );
        }

        SECTION( "Acquire bumps refcount; Release only frees at zero" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle h = table.Insert( Sample{ 7u } );
            table.Acquire( h );
            table.Acquire( h );
            REQUIRE( table.GetRefCount( h ) == 3u );

            REQUIRE_FALSE( table.Release( h ) );
            REQUIRE_FALSE( table.Release( h ) );
            REQUIRE( table.IsAlive( h ) );
            REQUIRE( table.GetRefCount( h ) == 1u );

            REQUIRE( table.Release( h ) );
            REQUIRE_FALSE( table.IsAlive( h ) );
        }

        SECTION( "Slot reuse invalidates the old handle" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle oldHandle = table.Insert( Sample{ 100u } );
            ( void )table.Release( oldHandle );

            const MeshHandle newHandle = table.Insert( Sample{ 200u } );

            REQUIRE_FALSE( table.IsAlive( oldHandle ) );
            REQUIRE( table.TryGet( oldHandle ) == nullptr );

            REQUIRE( table.IsAlive( newHandle ) );
            REQUIRE( table.Get( newHandle ).Value == 200u );
            REQUIRE( table.GetAliveCount() == 1 );
        }

        SECTION( "TryGet on invalid handle returns nullptr without asserting" )
        {
            RenderResourceTable< Sample > table;
            REQUIRE( table.TryGet( MeshHandle{} ) == nullptr );
        }

        SECTION( "Multiple inserts produce distinct handles" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle a = table.Insert( Sample{ 1u } );
            const MeshHandle b = table.Insert( Sample{ 2u } );
            const MeshHandle c = table.Insert( Sample{ 3u } );

            REQUIRE( a != b );
            REQUIRE( b != c );
            REQUIRE( table.Get( a ).Value == 1u );
            REQUIRE( table.Get( b ).Value == 2u );
            REQUIRE( table.Get( c ).Value == 3u );
            REQUIRE( table.GetAliveCount() == 3 );
        }

        SECTION( "Release on stale handle is a no-op" )
        {
            RenderResourceTable< Sample > table;
            const MeshHandle h = table.Insert( Sample{ 5u } );
            REQUIRE( table.Release( h ) );
            REQUIRE_FALSE( table.Release( h ) );
            REQUIRE( table.GetAliveCount() == 0 );
        }

        SECTION( "Clear resets the table" )
        {
            RenderResourceTable< Sample > table;
            ( void )table.Insert( Sample{ 1u } );
            ( void )table.Insert( Sample{ 2u } );
            REQUIRE( table.GetAliveCount() == 2 );

            table.Clear();
            REQUIRE( table.GetAliveCount() == 0 );
            REQUIRE( table.GetCapacity() == 0 );
        }
    }
}
