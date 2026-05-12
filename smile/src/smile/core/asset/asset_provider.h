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
 * @file        asset_provider.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Base class template for loading and caching transient assets
 */
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/core/fs/path.h"
#include "asset_loader.h"

namespace smile::asset
{
    template < AssetDerived TAsset, typename TKey, typename TLoader >
    requires std::is_base_of_v< AssetLoader< TAsset >, TLoader >
    class AssetProvider
    {
      public:
        using Ref = memory::Ref< TAsset >;

        AssetProvider() = default;

        template < typename... Args >
        explicit AssetProvider( Args &&...args ) noexcept : m_Loader{ std::forward< Args >( args )... }
        {
        }

        virtual ~AssetProvider() = default;

        Ref Get( const TKey &key ) const
        {
            if ( m_Assets.HasItemAtKey( key ) )
            {
                return m_Assets[key];
            }

            SM_LOG_WARNING( "AssetProvider::Get > Could not find asset with key: {}", key );

            return GetFallback();
        }

        Ref Load( const fs::Path &path )
        {
            Ref asset = m_Loader.Load( path );

            if ( asset )
            {
                Insert( asset );
                return asset;
            }

            SM_LOG_WARNING( "AssetProvider::Load > Could not load asset: {}", path );

            return GetFallback();
        }

        Ref GetOrLoad( const fs::Path &path )
        {
            Ref asset = Get( path );

            if ( asset )
            {
                return asset;
            }

            return Load( path );
        }

        virtual Ref GetFallback() const
        {
            return nullptr;
        }

      protected:
        bool Contains( const TKey &key ) const noexcept
        {
            return m_Assets.HasItemAtKey( key );
        }

        void Insert( Ref asset )
        {
            m_Assets.Insert( GetKey( asset ), asset );
        }

        virtual TKey GetKey( Ref asset ) const = 0;

      private:
        primitive::HashMap< TKey, Ref > m_Assets;
        TLoader m_Loader;
    };
}