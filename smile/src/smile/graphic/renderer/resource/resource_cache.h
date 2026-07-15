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
 * @file        resource_cache.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Generic resource cache for managing render resources
 */
#pragma once

#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    template < typename KeyType, typename ResourceType >
    class ResourceCache final
    {
      public:
        using Iterator = primitive::HashMap< KeyType, ResourceType >::Iterator;
        using ConstIterator = primitive::HashMap< KeyType, ResourceType >::ConstIterator;

        ResourceType Add( const KeyType &key, const ResourceType &resource )
        {
            Invalidate( key );
            auto it = m_Cache.Insert( key, resource );
            return it.GetItem();
        }

        bool Has( const KeyType &key ) const noexcept
        {
            return m_Cache.HasItemAtKey( key );
        }

        std::optional< ResourceType > Find( const KeyType &key ) const
        {
            auto it = m_Cache.FindItemAtKey( key );

            return it != m_Cache.end() ? std::optional< ResourceType >{ it.GetItem() } : std::nullopt;
        }

        void Invalidate( const KeyType &key )
        {
            auto it = m_Cache.FindItemAtKey( key );

            if ( it != m_Cache.end() )
                m_Cache.EraseItemAtKey( key );
        }

        void Invalidate( const ResourceType &resource )
        {
            auto it = std::find_if(
                m_Cache.begin(), m_Cache.end(), [&resource]( const auto &kv ) { return kv.Value == resource; } );

            if ( it != m_Cache.end() )
            {
                m_Cache.Erase( it );
            }
        }

        void Invalidate() noexcept
        {
            m_Cache.Clear();
        }

      private:
        primitive::HashMap< KeyType, ResourceType > m_Cache;
    };
}