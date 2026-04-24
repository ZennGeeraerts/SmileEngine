/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"
#include "frame_buffer_attachment.h"
#include "smile/graphic/rhi/viewport.h"

namespace smile::graphic
{
    class Framebuffer final
    {
      public:
        Framebuffer() = default;

        Framebuffer( rhi::FramebufferHandle handle,
            FramebufferAttachmentSetHandle attachmentSetHandle,
            const Uint32 width,
            const Uint32 height ) noexcept
            : m_Handle{ handle }, m_AttachmentSetHandle{ attachmentSetHandle }, m_Width{ width }, m_Height{ height }
        {
        }

        Framebuffer( const Framebuffer & ) = default;
        Framebuffer( Framebuffer && ) = default;

        ~Framebuffer() = default;

        Framebuffer &operator=( const Framebuffer & ) = default;
        Framebuffer &operator=( Framebuffer && ) noexcept = default;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        rhi::FramebufferHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        FramebufferAttachmentSetHandle GetAttachmentSetHandle() const noexcept
        {
            return m_AttachmentSetHandle;
        }

        [[nodiscard]] rhi::Viewport GetViewport( const float minZ = 0.0f, const float maxZ = 0.0f ) const
        {
            return rhi::Viewport{
                0.0f, static_cast< float >( m_Width ), 0.0f, static_cast< float >( m_Height ), minZ, maxZ };
        }

        bool operator==( const Framebuffer &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::FramebufferHandle m_Handle;
        FramebufferAttachmentSetHandle m_AttachmentSetHandle;
        Uint32 m_Width;
        Uint32 m_Height;

        friend class ResourceManager;
    };
}