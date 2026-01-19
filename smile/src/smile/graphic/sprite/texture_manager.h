/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/singleton.h"
#include "texture_loader.h"
#include "texture_asset.h"
#include "smile/core/fs/path.h"

namespace smile::graphic
{
    class TextureManager final : public memory::Counted, public foundation::Singleton< TextureManager >
    {
      public:
        TextureManager() = default;
        ~TextureManager() = default;

        memory::Ref< TextureAsset > GetTexture( asset::AssetHandle handle );
        memory::Ref< TextureAsset > GetTexture( const fs::Path &path );
        memory::Ref< TextureAsset > GetTexture( memory::Ref< Texture > pTexture ) const;

      private:
        std::unordered_map< memory::Ref< Texture >, memory::Ref< TextureAsset > > m_Textures;
        TextureLoader m_TextureLoader;
        memory::Ref< TextureAsset > m_pFallBackTexture;
    };
}