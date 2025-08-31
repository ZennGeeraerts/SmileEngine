/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "directx11_state_cache.h"
#include "resource/directx11_sampler_state.h"

namespace smile::graphic
{
    namespace detail
    {
        struct SamplerStateHasher final
        {
            foundation::HashCode operator()( const SamplerState &samplerState ) const
            {
                foundation::HashCode hash = 0;
                hash ^= std::hash< int >()( static_cast< int >( samplerState.Filtering ) );
                hash ^= std::hash< int >()( static_cast< int >( samplerState.AddressingU ) );
                hash ^= std::hash< int >()( static_cast< int >( samplerState.AddressingV ) );
                hash ^= std::hash< int >()( static_cast< int >( samplerState.AddressingW ) );
                return hash;
            }
        };

        struct SamplerStateComparer final
        {
            bool operator()( const SamplerState &lhs, const SamplerState &rhs ) const
            {
                return lhs.Filtering == rhs.Filtering && lhs.AddressingU == rhs.AddressingU &&
                       lhs.AddressingV == rhs.AddressingV && lhs.AddressingW == rhs.AddressingW;
            }
        };
    }

    using DirectX11SamplerStateCache = typename DirectX11StateCache< SamplerState,
        DirectX11SamplerState,
        detail::SamplerStateHasher,
        detail::SamplerStateComparer >;
}