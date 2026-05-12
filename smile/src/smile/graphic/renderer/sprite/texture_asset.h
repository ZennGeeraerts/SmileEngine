/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/core/asset/asset.h"
#include "smile/graphic/resource/image.h"

namespace smile::graphic
{
    class TextureAsset final : public asset::Asset< TextureAsset >
    {
      public:
        using Ref = memory::Ref< TextureAsset >;
        using ConstRef = memory::Ref< const TextureAsset >;

        TextureAsset( const primitive::String &id, Image::Ref image ) noexcept : m_ID{ id }, m_Image{ image }
        {
        }

        primitive::StringView GetID() const noexcept
        {
            return m_ID;
        }

        Image::ConstRef GetImage() const noexcept
        {
            return m_Image;
        }

      private:
        primitive::String m_ID;
        Image::Ref m_Image;
    };
}