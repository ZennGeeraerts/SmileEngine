/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "texture_manager.h"

#include "smile/graphic/renderer/resource/texture.h"

namespace smile::graphic
{
    TextureAsset::Ref TextureManager::GetTexture( const fs::Path &path )
    {
        TextureAsset::Ref pTextureAsset = m_TextureLoader.Load( path );

        if ( pTextureAsset )
        {
            // m_Textures.Insert( pTextureAsset->GetTexture(), pTextureAsset );
            return pTextureAsset;
        }

        SM_LOG_WARNING( "TextureManager::GetTexture > Could not load texture: {}", path );

        return m_pFallBackTexture;
    }
}