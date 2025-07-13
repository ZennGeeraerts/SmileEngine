/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "frame_buffer_attachment.h"

namespace smile::graphic
{
    class Framebuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Framebuffer >;

        Framebuffer( rhi::FramebufferHandle handle,
            std::initializer_list< FramebufferAttachment > colorAttachments,
            const FramebufferAttachment &depthAttachment )
            : m_Handle{ handle }, m_ColorAttachments{ colorAttachments }, m_DepthAttachment{ depthAttachment }
        {
        }

        ~Framebuffer() = default;

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::FramebufferHandle m_Handle;
        primitive::Vector< FramebufferAttachment > m_ColorAttachments;
        FramebufferAttachment m_DepthAttachment;

        friend class ResourceManager;
    };
}