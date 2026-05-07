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
    class TextureAsset final : public asset::Asset< TextureAsset >
    {
      public:
        using Ref = memory::Ref< TextureAsset >;
        using ConstRef = memory::Ref< const TextureAsset >;

        TextureAsset( const Texture &texture ) : m_Texture{ texture }
        {
        }

        Texture GetTexture() const
        {
            return m_Texture;
        }

      private:
        Texture m_Texture;
    };
}