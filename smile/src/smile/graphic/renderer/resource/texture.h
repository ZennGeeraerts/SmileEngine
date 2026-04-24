/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/rhi/resource/texture.h"

namespace smile::graphic
{
    class Texture final
    {
      public:
        Texture() = default;

        Texture( rhi::TextureHandle handle, const Uint32 width, const Uint32 height, const rhi::Format format ) noexcept
            : m_Handle{ handle }, m_Width{ width }, m_Height{ height }, m_Format{ format }
        {
        }

        Texture( const Texture & ) = default;
        Texture( Texture && ) = default;
        ~Texture() = default;

        Texture &operator=( const Texture & ) = default;
        Texture &operator=( Texture && ) noexcept = default;

        rhi::TextureHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        Uint32 GetWidth() const noexcept
        {
            return m_Width;
        }

        Uint32 GetHeight() const noexcept
        {
            return m_Height;
        }

        rhi::Format GetFormat() const noexcept
        {
            return m_Format;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        bool operator==( const Texture &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

        bool operator!=( const Texture &other ) const noexcept
        {
            return !( *this == other );
        }

      private:
        rhi::TextureHandle m_Handle;
        Uint32 m_Width = 0;
        Uint32 m_Height = 0;
        rhi::Format m_Format = rhi::Format::UNKNOWN;

        friend class ResourceManager;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::Texture >
    {
        smile::foundation::HashCode operator()( const smile::graphic::Texture &texture ) const noexcept
        {
            return texture.GetHandle().Hash();
        }
    };
}
