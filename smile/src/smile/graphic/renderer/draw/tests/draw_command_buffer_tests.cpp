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
 * @brief       Tests for DrawCommandBuffer sorting and lifecycle.
 */
#include "smile/graphic/renderer/draw/draw_command_buffer.h"
#include "smile/graphic/renderer/draw/sort_key.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    static DrawCommand MakeCommand( const SortKey key )
    {
        DrawCommand cmd;
        cmd.Key = key;
        return cmd;
    }

    TEST_CASE( "DrawCommandBuffer - basic usage", "[graphic][draw]" )
    {
        SECTION( "Empty buffer reports zero count and IsEmpty" )
        {
            DrawCommandBuffer buf;

            REQUIRE( buf.GetCommandCount() == 0 );
            REQUIRE( buf.IsEmpty() );
        }

        SECTION( "Emplace increases count" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 0, 1.0f ) ) );
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 2, 0, 2.0f ) ) );

            REQUIRE( buf.GetCommandCount() == 2 );
            REQUIRE_FALSE( buf.IsEmpty() );
        }

        SECTION( "Clear resets count to zero" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 0, 1.0f ) ) );
            buf.Clear();

            REQUIRE( buf.GetCommandCount() == 0 );
            REQUIRE( buf.IsEmpty() );
        }
    }

    TEST_CASE( "DrawCommandBuffer - Sort", "[graphic][draw]" )
    {
        SECTION( "Commands are sorted ascending by SortKey" )
        {
            DrawCommandBuffer buf;

            const SortKey hi = sort_key::EncodeOpaque( 3, 0, 50.0f );
            const SortKey mid = sort_key::EncodeOpaque( 2, 0, 10.0f );
            const SortKey lo = sort_key::EncodeOpaque( 1, 0, 1.0f );

            buf.Emplace( MakeCommand( hi ) );
            buf.Emplace( MakeCommand( mid ) );
            buf.Emplace( MakeCommand( lo ) );

            buf.Sort();

            const auto cmds = buf.GetCommands();
            REQUIRE( cmds.GetItemCount() == 3 );
            REQUIRE( cmds[0].Key == lo );
            REQUIRE( cmds[1].Key == mid );
            REQUIRE( cmds[2].Key == hi );
        }

        SECTION( "Sort on empty buffer is a no-op" )
        {
            DrawCommandBuffer buf;
            buf.Sort(); // must not crash
            REQUIRE( buf.IsEmpty() );
        }

        SECTION( "Sort on single command is a no-op" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 1, 5.0f ) ) );
            buf.Sort();
            REQUIRE( buf.GetCommandCount() == 1 );
        }

        SECTION( "Second Sort call without modification is idempotent" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 2, 0, 1.0f ) ) );
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 0, 2.0f ) ) );

            buf.Sort();
            buf.Sort(); // second call — must not change result

            const auto cmds = buf.GetCommands();
            REQUIRE( cmds[0].Key < cmds[1].Key );
        }

        SECTION( "Emplace after Sort marks buffer unsorted" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 2, 0, 1.0f ) ) );
            buf.Sort();
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 0, 1.0f ) ) );
            buf.Sort(); // must re-sort including the new entry

            const auto cmds = buf.GetCommands();
            REQUIRE( cmds[0].Key < cmds[1].Key );
        }

        SECTION( "Clear then repopulate and sort" )
        {
            DrawCommandBuffer buf;
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 5, 0, 1.0f ) ) );
            buf.Sort();
            buf.Clear();

            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 2, 0, 10.0f ) ) );
            buf.Emplace( MakeCommand( sort_key::EncodeOpaque( 1, 0, 5.0f ) ) );
            buf.Sort();

            const auto cmds = buf.GetCommands();
            REQUIRE( cmds.GetItemCount() == 2 );
            REQUIRE( cmds[0].Key < cmds[1].Key );
        }
    }
}
