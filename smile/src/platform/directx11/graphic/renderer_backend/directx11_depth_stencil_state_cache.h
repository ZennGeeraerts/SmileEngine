/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "directx11_state_cache.h"
#include "resource/directx11_depth_stencil_state.h"

namespace smile::graphic
{
    namespace detail
    {
        struct DepthStencilStateHasher final
        {
            foundation::HashCode operator()( const RenderState &renderState ) const
            {
                foundation::HashCode hash = 0;

                hash = foundation::HashCombine( hash, std::hash< bool >{}( renderState.DepthEnable ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( renderState.DepthWriteMask ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( renderState.DepthComparissonFunc ) ) );

                return hash;
            }
        };

        struct DepthStencilStateComparer final
        {
            bool operator()( const RenderState &lhs, const RenderState &rhs ) const
            {
                return lhs.DepthEnable == rhs.DepthEnable && lhs.DepthWriteMask == rhs.DepthWriteMask &&
                       lhs.DepthComparissonFunc == rhs.DepthComparissonFunc;
            }
        };
    }

    using DirectX11DepthStencilStateCache = typename DirectX11StateCache< RenderState,
        DirectX11DepthStencilState,
        detail::DepthStencilStateHasher,
        detail::DepthStencilStateComparer >;
}