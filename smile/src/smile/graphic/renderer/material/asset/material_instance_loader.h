#pragma once

#include "smile/core/asset/asset_loader.h"
#include "material_instance_asset.h"

namespace smile::graphic
{
    class TextureManager;
    class ShaderLibrary;
    class MaterialManager;

    class MaterialInstanceLoader final : public asset::AssetLoader
    {
      public:
        MaterialInstanceLoader( TextureManager &textureManager,
            ShaderLibrary &shaderLib,
            MaterialManager &materialManager ) noexcept;

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialInstanceAsset >() };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        MaterialInstanceAsset::Ref LoadMaterialInstance( const std::filesystem::path &path ) const;

      private:
        const std::vector< std::filesystem::path > m_Extensions{ ".smmati" };
        TextureManager &m_TextureManager;
        ShaderLibrary &m_ShaderLibrary;
        MaterialManager &m_MaterialManager;
    };
}