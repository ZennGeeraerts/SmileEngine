/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <unordered_map>

namespace smile::graphic::rhi
{
    template < typename KeyType,
        typename StateType,
        typename Hasher = std::hash< KeyType >,
        typename Comparer = std::equal_to< KeyType > >
    class D3D11StateCache final
    {
      public:
        const StateType *Add( const KeyType &state, Scope< StateType > pD3D11State )
        {
            Invalidate( state );
            auto pair = m_HashMap.emplace( state, std::move( pD3D11State ) );
            return pair.first->second.get();
        }

        const StateType *Find( const KeyType &state ) const
        {
            typename HashMap::const_iterator it = m_HashMap.find( state );

            if ( it != m_HashMap.end() )
                return it->second.get();

            return nullptr;
        }

        void Invalidate( const KeyType &state )
        {
            typename HashMap::const_iterator it = m_HashMap.find( state );

            if ( it != m_HashMap.end() )
                m_HashMap.erase( state );
        }

        void Invalidate()
        {
            m_HashMap.clear();
        }

      private:
        using HashMap = std::unordered_map< KeyType, Scope< StateType >, Hasher, Comparer >;

        HashMap m_HashMap;
    };
}