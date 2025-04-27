/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset_loader.h"

namespace smile::graphic
{
    class ShaderAsset;

    class ShaderLoader final : public asset::AssetLoader
    {
      public:
        ShaderLoader();

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< ShaderAsset >() };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        memory::Ref< ShaderAsset > LoadShader( const std::filesystem::path &path ) const;

      private:
        const std::vector< std::filesystem::path > m_Extensions{ ".smshader" };
    };
}