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
 * @file        draw_command_buffer_tests.cpp
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       Tests for DrawCommandBuffer binning and sort lifecycle.
 */
#include "smile/graphic/renderer/draw/draw_command_buffer.h"
#include "smile/graphic/renderer/draw/sort_key.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    namespace
    {
        const ecs::EntityHandle k_TestEntity{ 1u, 0u };
    }

    TEST_CASE( "DrawCommandBuffer - basic usage", "[graphic][draw]" )
    {
        SECTION( "Empty buffer reports zero count and IsEmpty" )
        {
            DrawCommandBuffer buf;

            REQUIRE( buf.GetBinCount() == 0 );
            REQUIRE( buf.IsEmpty() );
        }

        SECTION( "Adding entries with distinct sort keys creates one bin per key" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 1, 0, 1.0f ), k_TestEntity );
            buf.Add( sort_key::EncodeOpaque( 2, 0, 2.0f ), k_TestEntity );

            REQUIRE( buf.GetBinCount() == 2 );
            REQUIRE_FALSE( buf.IsEmpty() );
        }

        SECTION( "Same sort key merges into one bin with multiple entities" )
        {
            DrawCommandBuffer buf;
            const SortKey key = sort_key::EncodeOpaque( 1, 0, 1.0f );

            buf.Add( key, k_TestEntity );
            buf.Add( key, k_TestEntity );

            REQUIRE( buf.GetBinCount() == 1 );

            buf.Sort();
            REQUIRE( buf.GetBins()[0]->Entities.GetItemCount() == 2 );
        }

        SECTION( "Clear resets to empty" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 1, 0, 1.0f ), k_TestEntity );
            buf.Clear();

            REQUIRE( buf.GetBinCount() == 0 );
            REQUIRE( buf.IsEmpty() );
        }
    }

    TEST_CASE( "DrawCommandBuffer - Sort", "[graphic][draw]" )
    {
        SECTION( "Bins are sorted ascending by SortKey" )
        {
            DrawCommandBuffer buf;

            const SortKey hi = sort_key::EncodeOpaque( 3, 0, 50.0f );
            const SortKey mid = sort_key::EncodeOpaque( 2, 0, 10.0f );
            const SortKey lo = sort_key::EncodeOpaque( 1, 0, 1.0f );

            buf.Add( sort_key::EncodeOpaque( 3, 0, 50.0f ), k_TestEntity );
            buf.Add( sort_key::EncodeOpaque( 2, 0, 10.0f ), k_TestEntity );
            buf.Add( sort_key::EncodeOpaque( 1, 0, 1.0f ), k_TestEntity );

            buf.Sort();

            const auto &bins = buf.GetBins();
            REQUIRE( bins.GetItemCount() == 3 );
            REQUIRE( bins[0]->Key == lo );
            REQUIRE( bins[1]->Key == mid );
            REQUIRE( bins[2]->Key == hi );
        }

        SECTION( "Sort on empty buffer is a no-op" )
        {
            DrawCommandBuffer buf;
            buf.Sort(); // must not crash
            REQUIRE( buf.IsEmpty() );
        }

        SECTION( "Sort on single bin is a no-op" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 1, 1, 5.0f ), k_TestEntity );
            buf.Sort();
            REQUIRE( buf.GetBinCount() == 1 );
        }

        SECTION( "Second Sort call without modification is idempotent" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 2, 0, 1.0f ), k_TestEntity );
            buf.Add( sort_key::EncodeOpaque( 1, 0, 2.0f ), k_TestEntity );

            buf.Sort();
            buf.Sort(); // second call — must not change result

            const auto &bins = buf.GetBins();
            REQUIRE( bins[0]->Key < bins[1]->Key );
        }

        SECTION( "Add after Sort marks buffer unsorted" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 2, 0, 1.0f ), k_TestEntity );
            buf.Sort();
            buf.Add( sort_key::EncodeOpaque( 1, 0, 1.0f ), k_TestEntity );
            buf.Sort(); // must re-sort including the new bin

            const auto &bins = buf.GetBins();
            REQUIRE( bins[0]->Key < bins[1]->Key );
        }

        SECTION( "Clear then repopulate and sort" )
        {
            DrawCommandBuffer buf;
            buf.Add( sort_key::EncodeOpaque( 5, 0, 1.0f ), k_TestEntity );
            buf.Sort();
            buf.Clear();

            buf.Add( sort_key::EncodeOpaque( 2, 0, 10.0f ), k_TestEntity );
            buf.Add( sort_key::EncodeOpaque( 1, 0, 5.0f ), k_TestEntity );
            buf.Sort();

            const auto &bins = buf.GetBins();
            REQUIRE( bins.GetItemCount() == 2 );
            REQUIRE( bins[0]->Key < bins[1]->Key );
        }
    }
}
