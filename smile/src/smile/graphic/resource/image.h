/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/object.h"

namespace smile::graphic
{
    enum class ImageFormat : Uint8
    {
        RGB,
        RGBA,
        BGRA,
        BC1,
        BC2,
        BC3,
        BC4,
        BC5,
        BC6,
        BC7,
        RG32F,
        Count,
        None
    };

    class Image final : public memory::Object
    {
      public:
        Image();
        Image( const Uint32 width, const Uint32 height, const ImageFormat format, const Byte *pData );

        Uint32 GetWidth() const
        {
            return m_Width;
        }

        Uint32 GetHeight() const
        {
            return m_Height;
        }

        ImageFormat GetFormat() const
        {
            return m_Format;
        }

        const Byte *GetData() const
        {
            return m_Buffer.data();
        }

        Uint32 GetDataSize() const
        {
            return static_cast< Uint32 >( m_Buffer.size() );
        }

      private:
        Uint32 m_Width;
        Uint32 m_Height;
        ImageFormat m_Format;
        std::vector< Byte > m_Buffer;
        Uint32 m_Stride;
    };
}