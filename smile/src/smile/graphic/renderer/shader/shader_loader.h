/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset_loader.h"

namespace smile::graphic
{
    class ShaderAsset;
    class ShaderReflectionData;

    class ShaderLoader final : public asset::AssetLoader
    {
      public:
        ShaderLoader();

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< ShaderAsset >() };
        }

        const primitive::Vector< fs::Path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        memory::Ref< ShaderAsset > LoadShader( const fs::Path &path ) const;

      private:
        bool DeserializeReflectionData( const std::string &yamlContent, ShaderReflectionData &reflectionData ) const;

      private:
        const primitive::Vector< fs::Path > m_Extensions{ ".smshader" };
    };
}