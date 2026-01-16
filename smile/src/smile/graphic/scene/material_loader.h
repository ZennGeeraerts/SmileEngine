#pragma once
#include "smile/core/asset/asset_loader.h"
#include "material_asset.h"

namespace smile::graphic
{
    class MaterialLoader final : public asset::AssetLoader
    {
      public:
        MaterialLoader();

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialAsset >() };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        MaterialAsset::Ref LoadMaterial( const std::filesystem::path &path ) const;

      private:
        const std::vector< std::filesystem::path > m_Extensions{ ".png", ".jpg", ".dds" };
    };
}