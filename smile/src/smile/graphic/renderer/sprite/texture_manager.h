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
    class TextureManager final
    {
      public:
        TextureManager() = default;
        ~TextureManager() = default;

        TextureAsset::Ref GetTexture( asset::AssetHandle handle );
        TextureAsset::Ref GetTexture( const fs::Path &path );

        TextureAsset::Ref GetFallBackTexture() const
        {
            return m_pFallBackTexture;
        }

      private:
        TextureLoader m_TextureLoader;
        TextureAsset::Ref m_pFallBackTexture;
    };
}