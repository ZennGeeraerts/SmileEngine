/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset_provider.h"
#include "texture_loader.h"

namespace smile::graphic
{
    class TextureManager final : public asset::AssetProvider< TextureAsset, primitive::String, TextureLoader >
    {
      public:
        TextureManager() noexcept : m_pFallBackTexture{ nullptr }
        {
        }

        void LoadFallback( const fs::Path &path )
        {
            m_pFallBackTexture = m_Loader.Load( path );

            SM_ASSERT( m_pFallBackTexture );
        }

        TextureAsset::Ref GetFallback() const override
        {
            return m_pFallBackTexture;
        }

      protected:
        primitive::String GetKey( TextureAsset::Ref asset ) const override
        {
            return asset->GetID();
        }

      private:
        TextureAsset::Ref m_pFallBackTexture;
    };
}