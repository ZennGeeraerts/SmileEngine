/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/array.h"
#include "smile/core/asset/asset_loader.h"
#include "shader_asset.h"

namespace smile::graphic
{
    class ShaderLoader final : public asset::AssetLoader< ShaderAsset >
    {
      public:
        ShaderLoader();

        primitive::ArrayView< const fs::Path > GetExtensions() const override
        {
            return m_Extensions.AsView();
        }

        memory::Ref< asset::BaseAsset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        ShaderAsset::Ref Load( const fs::Path &path ) const override;

      private:
        const primitive::Array< fs::Path, 1 > m_Extensions{ ".smshader" };
    };
}