/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/frame_buffer.h"
#include "frame_buffer_attachment.h"

namespace smile::graphic
{
    class Framebuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Framebuffer >;
        using ConstRef = memory::Ref< const Framebuffer >;

        Framebuffer( rhi::FramebufferHandle handle,
            const primitive::Vector< FramebufferAttachment > &colorAttachments,
            const FramebufferAttachment &depthAttachment,
            const rhi::FramebufferInfoExtented &info )
            : m_Handle{ handle },
              m_ColorAttachments{ colorAttachments },
              m_DepthAttachment{ depthAttachment },
              m_Info{ info }
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

        [[nodiscard]] rhi::Viewport GetViewport( const float minZ = 0.0f, const float maxZ = 0.0f ) const
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