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
    void TextureManager::Initialize( ResourceManager *resourceManager ) noexcept
    {
        m_TextureLoader.Initialize( resourceManager );
    }

    TextureAsset::Ref TextureManager::GetTexture( asset::AssetHandle handle )
    {
        TextureAsset::Ref pTextureAsset = asset::AssetManager::GetAsset< TextureAsset >( handle );

        if ( pTextureAsset )
        {
            m_Textures.Insert( pTextureAsset->GetTexture(), pTextureAsset );
            return pTextureAsset;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not load texture: {}", static_cast< Uint64 >( handle ) );

        return m_pFallBackTexture;
    }

    TextureAsset::Ref TextureManager::GetTexture( const std::filesystem::path &path )
    {
        TextureAsset::Ref pTextureAsset = m_TextureLoader.LoadTexture( path );

        if ( pTextureAsset )
        {
            m_Textures.Insert( pTextureAsset->GetTexture(), pTextureAsset );
            return pTextureAsset;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not load texture: {}", path.string() );

        return m_pFallBackTexture;
    }

    TextureAsset::Ref TextureManager::GetTexture( Texture::Ref pTexture ) const
    {
        auto it = m_Textures.FindItemAtKey( pTexture );

        if ( it != m_Textures.end() )
        {
            return it.GetItem();
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not find texture in texture map: {}",
            pTexture->GetHandle().GetIndex() );

        return m_pFallBackTexture;
    }
}