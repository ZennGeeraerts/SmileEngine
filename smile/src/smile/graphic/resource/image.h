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

        Image() noexcept;
        Image( const Uint32 width, const Uint32 height, const rhi::Format format, const Byte *pData );

        [[nodiscard]] Uint32 GetWidth() const noexcept
        {
            return m_Width;
        }

        [[nodiscard]] Uint32 GetHeight() const noexcept
        {
            return m_Height;
        }

        [[nodiscard]] rhi::Format GetFormat() const noexcept
        {
            return m_Format;
        }

        [[nodiscard]] const Byte *GetData() const noexcept
        {
            return m_Buffer.GetData();
        }

        [[nodiscard]] Count GetDataSize() const noexcept
        {
            return m_Buffer.GetItemCount();
        }

        [[nodiscard]] Count GetBytesPerPixel() const noexcept
        {
            return m_BytesPerPixel;
        }

        [[nodiscard]] Count GetRowStride() const noexcept
        {
            return m_BytesPerPixel * m_Width;
        }

        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return m_Buffer.IsEmpty();
        }

      private:
        Uint32 m_Width;
        Uint32 m_Height;
        rhi::Format m_Format;
        primitive::Vector< Byte > m_Buffer;
        Count m_BytesPerPixel;
    };
}