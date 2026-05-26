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
 * @file        frame_arena_tests.cpp
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Tests for FrameArena bump-pointer allocator
 */
#include "smile/graphic/renderer/memory/frame_arena.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    namespace
    {
        struct alignas( 32 ) AlignedThing final
        {
            Uint64 a;
            Uint64 b;
        };

        struct PodSample final
        {
            Uint32 x;
            float y;
        };
    }

    TEST_CASE( "FrameArena", "[graphic][memory]" )
    {
        SECTION( "Allocates default-initialised typed span" )
        {
            FrameArena arena{ 1024 };

            auto span = arena.Allocate< Uint32 >( 16 );
            REQUIRE( span.GetItemCount() == 16 );
            REQUIRE( span.GetData() != nullptr );

            for ( Index i = 0; i < span.GetItemCount(); ++i )
                REQUIRE( span[i] == 0u );
        }

        SECTION( "Respects custom alignment" )
        {
            FrameArena arena{ 1024 };

            // Bump the offset off-alignment first.
            ( void )arena.Allocate< Uint8 >( 1 );
            auto aligned = arena.Allocate< AlignedThing >( 1 );

            const auto address = reinterpret_cast< UintPointer >( aligned.GetData() );
            REQUIRE( ( address % alignof( AlignedThing ) ) == 0 );
        }

        SECTION( "Allocations within a chunk are unique pointers" )
        {
            FrameArena arena{ 1024 };

            auto a = arena.Allocate< Uint32 >( 4 );
            auto b = arena.Allocate< Uint32 >( 4 );

            REQUIRE( a.GetData() != b.GetData() );
            REQUIRE( b.GetData() > a.GetData() );
        }

        SECTION( "Grows on demand when current chunk is exhausted" )
        {
            FrameArena arena{ 64 };

            // First allocation fits in the initial chunk.
            ( void )arena.Allocate< Uint8 >( 32 );
            REQUIRE( arena.GetChunkCount() == 1 );

            // Second allocation overflows; arena must allocate a new chunk.
            ( void )arena.Allocate< Uint8 >( 64 );
            REQUIRE( arena.GetChunkCount() >= 2 );
        }

        SECTION( "Oversized allocation creates a chunk large enough to fit" )
        {
            FrameArena arena{ 64 };

            const Count bigSize = 4096;
            auto big = arena.Allocate< Uint8 >( bigSize );
            REQUIRE( big.GetItemCount() == bigSize );

            // Subsequent small allocation succeeds.
            auto small = arena.Allocate< Uint32 >( 1 );
            REQUIRE( small.GetItemCount() == 1 );
        }

        SECTION( "Reset rewinds without freeing chunks" )
        {
            FrameArena arena{ 256 };

            ( void )arena.Allocate< Uint8 >( 128 );
            ( void )arena.Allocate< Uint8 >( 200 ); // forces second chunk

            const Count chunkCountBefore = arena.GetChunkCount();
            REQUIRE( chunkCountBefore >= 2 );
            REQUIRE( arena.GetUsedSize() > 0 );

            arena.Reset();

            REQUIRE( arena.GetChunkCount() == chunkCountBefore );
            REQUIRE( arena.GetUsedSize() == 0 );

            // Re-allocation after reset reuses the first chunk.
            auto span = arena.Allocate< Uint8 >( 64 );
            REQUIRE( span.GetItemCount() == 64 );
        }

        SECTION( "EmplaceSpan copies initializer list contents" )
        {
            FrameArena arena{ 256 };

            auto span = arena.EmplaceSpan< PodSample >( { { 1u, 1.5f }, { 2u, 2.5f }, { 3u, 3.5f } } );
            REQUIRE( span.GetItemCount() == 3 );
            REQUIRE( span[0].x == 1u );
            REQUIRE( span[1].x == 2u );
            REQUIRE( span[2].y == 3.5f );
        }

        SECTION( "ReleaseChunks frees everything" )
        {
            FrameArena arena{ 256 };
            ( void )arena.Allocate< Uint8 >( 100 );
            REQUIRE( arena.GetChunkCount() == 1 );

            arena.ReleaseChunks();
            REQUIRE( arena.GetChunkCount() == 0 );
            REQUIRE( arena.GetCommittedSize() == 0 );
        }
    }
}
