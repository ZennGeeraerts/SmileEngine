/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/memory/ref.h"
#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    class Texture final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Texture >;

        Texture( TextureHandle handle, const Uint32 width, const Uint32 height )
            : m_Handle{ handle }, m_Width{ width }, m_Height{ height }
        {
        }

        ~Texture() = default;
        Texture( const Texture & ) = delete;
        Texture( Texture && ) = delete;

        Uint32 GetWidth() const
        {
            return m_Width;
        }

        Uint32 GetHeight() const
        {
            return m_Height;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        TextureHandle m_Handle;
        Uint32 m_Width;
        Uint32 m_Height;

        friend class ResourceManager;
    };
}
