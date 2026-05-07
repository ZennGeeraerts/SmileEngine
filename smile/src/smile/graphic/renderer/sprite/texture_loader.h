/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/array.h"
#include "smile/core/asset/asset_loader.h"
#include "texture_asset.h"

namespace smile::graphic
{
    class ResourceManager;

    class TextureLoader final : public asset::AssetLoader< TextureAsset >
    {
      public:
        TextureLoader( ResourceManager &resourceManager ) noexcept;

        primitive::ArrayView< const fs::Path > GetExtensions() const override
        {
            return m_Extensions.AsView();
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        memory::Ref< TextureAsset > Load( const fs::Path &path ) const override;

      private:
        const primitive::Array< fs::Path, 3 > m_Extensions{ ".png", ".jpg", ".dds" };
        ResourceManager &m_ResourceManager;
    };
}