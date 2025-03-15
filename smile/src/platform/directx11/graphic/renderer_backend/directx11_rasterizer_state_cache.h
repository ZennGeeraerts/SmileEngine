/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "smile/graphic/renderer_backend/render_state.h"
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

    class DirectX11RasterizerStateCache final
    {
      public:
        const DirectX11RasterizerState *Add( const RenderState &state, Scope< DirectX11RasterizerState > pRasterizerState );
        const DirectX11RasterizerState *Find( const RenderState &state ) const;
        void Invalidate( const RenderState &state );
        void Invalidate();

      private:
        using HashMap = std::unordered_map< RenderState,
            Scope< DirectX11RasterizerState >,
            detail::RasterizerStateHasher,
            detail::RasterizerStateComparer >;

        HashMap m_HashMap;
    };
}