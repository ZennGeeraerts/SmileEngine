/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/hash_map.h"
#include "texture_loader.h"
#include "texture_asset.h"

namespace smile::graphic
{
    class ResourceManager;

    class TextureManager final
    {
      public:
        TextureManager() = default;
        ~TextureManager() = default;

        void Initialize( ResourceManager *resourceManager ) noexcept;

        TextureAsset::Ref GetTexture( asset::AssetHandle handle );
        TextureAsset::Ref GetTexture( const std::filesystem::path &path );
        TextureAsset::Ref GetTexture( Texture::Ref pTexture ) const;

        TextureAsset::Ref GetFallBackTexture() const
        {
            return m_pFallBackTexture;
        }

      private:
        primitive::HashMap< Texture::Ref, TextureAsset::Ref > m_Textures;
        TextureLoader m_TextureLoader;
        TextureAsset::Ref m_pFallBackTexture;
    };
}