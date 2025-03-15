/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_rasterizer_state_cache.h"

namespace smile::graphic
{
    const DirectX11RasterizerState *DirectX11RasterizerStateCache::Add( const RenderState &state,
        Scope< DirectX11RasterizerState > pRasterizerState )
    {
        Invalidate( state );
        auto pair = m_HashMap.emplace( state, std::move( pRasterizerState ) );
        return pair.first->second.get();
    }

    const DirectX11RasterizerState *DirectX11RasterizerStateCache::Find( const RenderState &state ) const
    {
        typename HashMap::const_iterator it = m_HashMap.find( state );

        if ( it != m_HashMap.end() )
            return it->second.get();

        return nullptr;
    }

    void DirectX11RasterizerStateCache::Invalidate( const RenderState &state )
    {
        typename HashMap::const_iterator it = m_HashMap.find( state );

        if ( it != m_HashMap.end() )
            m_HashMap.erase( state );
    }

    void DirectX11RasterizerStateCache::Invalidate()
    {
        m_HashMap.clear();
    }
}