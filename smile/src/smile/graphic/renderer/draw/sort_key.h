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
 * @file        sort_key.h
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       64-bit sort key encoders for mesh-pass draw command lists.
 *
 *              Draw commands are sorted by an opaque 64-bit key. Encoders pack the
 *              fields whose relative ordering matters most into the most-significant
 *              bits; sorting ascending then groups by that field first.  Three
 *              standard encoders are provided:
 *
 *                Opaque       [PSO 16][Material 16][Depth 32]
 *                              -> state-sort first, draw nearest-first within state
 *
 *                Transparent  [InvDepth 32][PSO 16][Material 16]
 *                              -> draw farthest-first (back-to-front), state-sort secondary
 *
 *                Shadow       [PSO 16][Material 16][Reserved 32]
 *                              -> state-sort; depth doesn't matter (no early-z benefit)
 *
 *              Depth values are encoded by bit-casting the non-negative float to
 *              Uint32 — for positive values IEEE-754 ordering matches integer ordering,
 *              so no scaling/quantisation is required.
 */
#pragma once

#include "smile/common/foundation/compiled.h"

#include <bit>

namespace smile::graphic
{
    /** Opaque 64-bit sort key. Smaller value sorts first. */
    struct SortKey final
    {
        Uint64 Value = 0;

        constexpr bool operator<( const SortKey other ) const noexcept
        {
            return Value < other.Value;
        }

        constexpr bool operator==( const SortKey other ) const noexcept
        {
            return Value == other.Value;
        }

        constexpr bool operator!=( const SortKey other ) const noexcept
        {
            return Value != other.Value;
        }
    };

    namespace sort_key
    {
        /** Maximum PSO / material index that fits in the 16-bit field of a sort key. */
        inline constexpr Index s_MaxFieldIndex = 0xFFFFu;

        /** Bit-cast a non-negative float depth to its monotonic Uint32 representation. */
        [[nodiscard]] constexpr Uint32 EncodeDepth( const float depth ) noexcept
        {
            // IEEE-754: for non-negative floats, increasing value -> increasing integer bits.
            return std::bit_cast< Uint32 >( depth );
        }

        /**
         * Opaque pass key: PSO (high 16) | Material (mid 16) | Depth (low 32).
         * Smaller key sorts first; sort first by PSO, then material, then near-to-far depth.
         */
        [[nodiscard]] constexpr SortKey
        EncodeOpaque( const Index psoIndex, const Index materialIndex, const float depth ) noexcept
        {
            SM_ASSERT( psoIndex <= s_MaxFieldIndex );
            SM_ASSERT( materialIndex <= s_MaxFieldIndex );

            const Uint64 pso = static_cast< Uint64 >( psoIndex ) << 48;
            const Uint64 mat = static_cast< Uint64 >( materialIndex ) << 32;
            const Uint64 dep = static_cast< Uint64 >( EncodeDepth( depth ) );
            return SortKey{ pso | mat | dep };
        }

        /**
         * Transparent pass key: InvDepth (high 32) | PSO (mid 16) | Material (low 16).
         * Sorted by farthest depth first; ties broken by PSO then material.
         */
        [[nodiscard]] constexpr SortKey
        EncodeTransparent( const Index psoIndex, const Index materialIndex, const float depth ) noexcept
        {
            SM_ASSERT( psoIndex <= s_MaxFieldIndex );
            SM_ASSERT( materialIndex <= s_MaxFieldIndex );

            const Uint64 invDepth = static_cast< Uint64 >( ~EncodeDepth( depth ) ) << 32;
            const Uint64 pso = static_cast< Uint64 >( psoIndex ) << 16;
            const Uint64 mat = static_cast< Uint64 >( materialIndex );
            return SortKey{ invDepth | pso | mat };
        }

        /**
         * Shadow pass key: PSO (high 16) | Material (mid 16) | low 32 bits unused.
         * Depth is irrelevant for shadow passes — only state coherence matters.
         */
        [[nodiscard]] constexpr SortKey EncodeShadow( const Index psoIndex, const Index materialIndex ) noexcept
        {
            SM_ASSERT( psoIndex <= s_MaxFieldIndex );
            SM_ASSERT( materialIndex <= s_MaxFieldIndex );

            const Uint64 pso = static_cast< Uint64 >( psoIndex ) << 48;
            const Uint64 mat = static_cast< Uint64 >( materialIndex ) << 32;
            return SortKey{ pso | mat };
        }
    }
}
