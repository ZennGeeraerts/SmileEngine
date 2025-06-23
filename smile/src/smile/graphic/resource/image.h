/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/format.h"

namespace smile::graphic
{
    class Image final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Image >;
        using ConstRef = memory::Ref< const Image >;

        Image();
        Image( const Uint32 width, const Uint32 height, const Format format, const Byte *pData );

        Uint32 GetWidth() const
        {
            return m_Width;
        }

        Uint32 GetHeight() const
        {
            return m_Height;
        }

        Format GetFormat() const
        {
            return m_Format;
        }

        const Byte *GetData() const
        {
            return m_Buffer.GetData();
        }

        Count GetDataSize() const
        {
            return m_Buffer.GetItemCount();
        }

        Uint32 GetStride() const
        {
            return m_Stride;
        }

      private:
        Uint32 m_Width;
        Uint32 m_Height;
        Format m_Format;
        primitive::Vector< Byte > m_Buffer;
        Uint32 m_Stride;
    };
}