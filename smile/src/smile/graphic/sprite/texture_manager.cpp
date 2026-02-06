/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "texture_manager.h"

#include "smile/core/asset/asset_manager.h"
#include "smile/graphic/renderer/resource/texture.h"

namespace smile::graphic
{
    memory::Ref< TextureAsset > TextureManager::GetTexture( asset::AssetHandle handle )
    {
        memory::Ref< TextureAsset > pTextureAsset = asset::AssetManager::GetAsset< TextureAsset >( handle );

        if ( pTextureAsset )
        {
            m_Textures.insert( std::make_pair( pTextureAsset->GetTexture(), pTextureAsset ) );
            return pTextureAsset;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not load texture: {}", static_cast< Uint64 >( handle ) );

        return m_pFallBackTexture;
    }

    memory::Ref< TextureAsset > TextureManager::GetTexture( const std::filesystem::path &path )
    {
        memory::Ref< TextureAsset > pTextureAsset = m_TextureLoader.LoadTexture( path );

        if ( pTextureAsset )
        {
            m_Textures.insert( std::make_pair( pTextureAsset->GetTexture(), pTextureAsset ) );
            return pTextureAsset;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not load texture: {}", path.string() );

        return m_pFallBackTexture;
    }

    memory::Ref< TextureAsset > TextureManager::GetTexture( memory::Ref< Texture > pTexture ) const
    {
        auto it = m_Textures.find( pTexture );

        if ( it != m_Textures.end() )
        {
            return it->second;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not find texture in texture map: {}",
            pTexture->GetHandle().GetIndex() );

        return m_pFallBackTexture;
    }
}