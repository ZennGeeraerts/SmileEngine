/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"
#include "smile/core/asset/asset_provider.h"
#include "shader_loader.h"

namespace smile::graphic
{
    class ShaderLibrary final : public asset::AssetProvider< ShaderAsset, primitive::String, ShaderLoader >
    {
      public:
        ShaderLibrary() = default;
        ~ShaderLibrary() = default;

      protected:
        primitive::String GetKey( ShaderAsset::Ref asset ) const override
        {
            return asset->GetName();
        }
    };
}