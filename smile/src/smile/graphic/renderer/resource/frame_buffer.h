/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_handle.h"
#include "frame_buffer_attachment.h"

namespace smile::graphic
{
    class Framebuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Framebuffer >;

        Framebuffer( rhi::FramebufferHandle handle,
            const primitive::Vector< FramebufferAttachment > &colorAttachments,
            const FramebufferAttachment &depthAttachment )
            : m_Handle{ handle }, m_ColorAttachments{ colorAttachments }, m_DepthAttachment{ depthAttachment }
        {
        }

        ~Framebuffer() = default;

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

        rhi::FramebufferHandle GetHandle() const
        {
            return m_Handle;
        }

        const primitive::Vector< FramebufferAttachment > &GetColorAttachments() const
        {
            return m_ColorAttachments;
        }

        const FramebufferAttachment &GetDepthAttachment() const
        {
            return m_DepthAttachment;
        }

      private:
        rhi::FramebufferHandle m_Handle;
        primitive::Vector< FramebufferAttachment > m_ColorAttachments;
        FramebufferAttachment m_DepthAttachment;

        friend class ResourceManager;
    };
}