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
 * @file        bin_key.h
 * @author      Zenn Geeraerts
 * @created     03 June 2026
 * @brief       Defines the BinKey struct and encoder for DrawBins in the DrawCommandBuffer.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/hash_code.h"

namespace smile::graphic
{
    struct BinKey final
    {
        Uint64 Value{ 0 };

        foundation::HashCode GetHashCode() const noexcept
        {
            return std::hash< Uint64 >{}( Value );
        }

        constexpr bool operator<( const BinKey other ) const noexcept
        {
            return Value < other.Value;
        }

        constexpr bool operator==( const BinKey other ) const noexcept
        {
            return Value == other.Value;
        }

        constexpr bool operator!=( const BinKey other ) const noexcept
        {
            return Value != other.Value;
        }
    };

    namespace bin_key
    {
        inline constexpr Index s_MaxFieldIndex = 0xFFFFu;

        [[nodiscard]] constexpr BinKey
        EncodeBin( const Index psoIndex, const Index materialIndex, const Index meshIndex ) noexcept
        {
            SM_ASSERT( psoIndex <= s_MaxFieldIndex );
            SM_ASSERT( materialIndex <= s_MaxFieldIndex );
            SM_ASSERT( meshIndex <= s_MaxFieldIndex );

            const Uint64 pso = static_cast< Uint64 >( psoIndex ) << 48;
            const Uint64 mat = static_cast< Uint64 >( materialIndex ) << 32;
            const Uint64 mesh = static_cast< Uint64 >( meshIndex );
            return BinKey{ pso | mat | mesh };
        }
    }
}

namespace std
{
    template <>
    struct hash< smile::graphic::BinKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::BinKey key ) const noexcept
        {
            return key.GetHashCode();
        }
    };
}