/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/core/asset/asset.h"
#include "smile/graphic/renderer/resource/texture.h"

namespace smile::graphic
{
    class TextureAsset final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< TextureAsset >;
        using ConstRef = memory::Ref< const TextureAsset >;

        TextureAsset( memory::Ref< Texture > pTexture ) : m_pTexture{ pTexture }
        {
        }

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< TextureAsset >() };
        }

        memory::Ref< Texture > GetTexture() const
        {
            return m_pTexture;
        }

      private:
        memory::Ref< Texture > m_pTexture;
    };
}