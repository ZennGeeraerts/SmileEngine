/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset.h"
#include "shader_reflection_data.h"

namespace smile::graphic
{
    class ShaderAsset final : public asset::Asset
    {
      public:
        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< ShaderAsset >() };
        }

      private:
        ShaderReflectionData m_ReflectionData;
    };
}