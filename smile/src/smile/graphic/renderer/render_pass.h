#pragma once

#include "smile/common/memory/counted.h"
#include "camera.h"
#include "resource/frame_buffer.h"

namespace smile::graphic
{
    class RenderPass : public memory::Counted
    {
      public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        RenderPass( const RenderPass & ) = delete;
        RenderPass( RenderPass && ) = delete;
        RenderPass &operator=( const RenderPass & ) = delete;
        RenderPass &operator=( RenderPass && ) = delete;

        virtual void
        OnRender( Framebuffer::Ref framebuffer, const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) = 0;
    };
}