/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class Framebuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Framebuffer >;

        Framebuffer( FramebufferHandle handle, const Uint32 width, const Uint32 height )
            : m_Handle{ handle }, m_Width{ width }, m_Height{ height }
        {
        }

        ~Framebuffer() = default;

        Uint32 GetWidth() const
        {
            return m_Width;
        }

        Uint32 GetHeight() const
        {
            return m_Width;
        }

      private:
        FramebufferHandle m_Handle;
        Uint32 m_Width{};
        Uint32 m_Height{};

        friend class ResourceManager;
    };
}