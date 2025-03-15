/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "directx11_state_cache.h"
#include "resource/directx11_rasterizer_state.h"

#include <unordered_map>

namespace smile::graphic
{
    namespace detail
    {
        struct RasterizerStateHasher final
        {
            foundation::HashCode operator()( const RenderState &renderState ) const
            {
                foundation::HashCode hash = 0;
                hash ^= std::hash< int >()( static_cast< int >( renderState.CullMode ) );
                hash ^= std::hash< int >()( static_cast< int >( renderState.FillMode ) );
                hash ^= std::hash< bool >()( renderState.EnableDepthClip );
                return hash;
            }
        };

        struct RasterizerStateComparer final
        {
            bool operator()( const RenderState &lhs, const RenderState &rhs ) const
            {
                return lhs.CullMode == rhs.CullMode && lhs.FillMode == rhs.FillMode &&
                       lhs.EnableDepthClip == rhs.EnableDepthClip;
            }
        };
    }

    using DirectX11RasterizerStateCache = typename DirectX11StateCache< DirectX11RasterizerState,
        detail::RasterizerStateHasher,
        detail::RasterizerStateComparer >;
}