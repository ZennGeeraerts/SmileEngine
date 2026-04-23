/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "frame_buffer_attachment.h"

namespace smile::graphic
{
    class Framebuffer final
    {
      public:
        Framebuffer() = default;

        Framebuffer( rhi::FramebufferHandle handle,
            const primitive::Vector< FramebufferAttachment > &colorAttachments,
            const FramebufferAttachment &depthAttachment,
            const rhi::FramebufferInfoExtented &info ) noexcept
            : m_Handle{ handle },
              m_ColorAttachments{ colorAttachments },
              m_DepthAttachment{ depthAttachment },
              m_Info{ info }
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

        const primitive::Vector< FramebufferAttachment > &GetColorAttachments() const noexcept
        {
            return m_ColorAttachments;
        }

        const FramebufferAttachment &GetDepthAttachment() const noexcept
        {
            return m_DepthAttachment;
        }

        [[nodiscard]] rhi::Viewport GetViewport( const float minZ = 0.0f, const float maxZ = 0.0f ) const noexcept
        {
            return m_Info.GetViewport( minZ, maxZ );
        }

      private:
        rhi::FramebufferHandle m_Handle;
        primitive::Vector< FramebufferAttachment > m_ColorAttachments;
        FramebufferAttachment m_DepthAttachment;
        rhi::FramebufferInfoExtented m_Info;

        friend class ResourceManager;
    };
}