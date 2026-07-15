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
 * @file        render_ids_tests.cpp
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Tests for render-world handle aliases
 */
#include "smile/graphic/renderer/scene/render_ids.h"

#include <catch/catch.hpp>

namespace smile::graphic
{
    TEST_CASE( "RenderHandle aliases", "[graphic][scene]" )
    {
        SECTION( "Default-constructed render handle is invalid" )
        {
            const MeshHandle handle{};
            REQUIRE_FALSE( handle.IsValid() );
        }

        SECTION( "Constructed handle is valid and round-trips its index" )
        {
            const MeshHandle handle{ 42u, 1u };
            REQUIRE( handle.IsValid() );
            REQUIRE( handle.GetIndex() == 42u );
        }

        SECTION( "Aliases share the same underlying type" )
        {
            STATIC_REQUIRE( std::is_same_v< MeshHandle, MaterialHandle > );
            STATIC_REQUIRE( std::is_same_v< RenderEntityId, LightHandle > );
            STATIC_REQUIRE( std::is_same_v< MeshHandle, RenderHandle > );
        }
    }

    TEST_CASE( "TextureBindlessIndex", "[graphic][scene]" )
    {
        SECTION( "Invalid sentinel is reserved" )
        {
            REQUIRE( static_cast< Uint32 >( TextureBindlessIndex::Invalid ) == std::numeric_limits< Uint32 >::max() );
        }
    }
}
