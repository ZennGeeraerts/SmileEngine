/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/format.h"

namespace smile::graphic
{
    class Texture final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Texture >;

        Texture( rhi::TextureHandle handle, const Uint32 width, const Uint32 height, const rhi::Format format )
            : m_Handle{ handle }, m_Width{ width }, m_Height{ height }, m_Format{ format }
        {
        }

        ~Texture() = default;
        Texture( const Texture & ) = delete;
        Texture( Texture && ) = delete;

        rhi::TextureHandle GetHandle() const
        {
            return m_Handle;
        }

        Uint32 GetWidth() const
        {
            return m_Width;
        }

        Uint32 GetHeight() const
        {
            return m_Height;
        }

        rhi::Format GetFormat() const
        {
            return m_Format;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::TextureHandle m_Handle;
        Uint32 m_Width;
        Uint32 m_Height;
        rhi::Format m_Format;

        friend class ResourceManager;
    };
}
