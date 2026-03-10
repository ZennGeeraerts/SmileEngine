/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "render_pass.h"

namespace smile::graphic::ecs
{
    class DebugRenderPass final : public RenderPass
    {
      public:
        DebugRenderPass() = default;

        DebugRenderPass( const DebugRenderPass & ) = delete;
        DebugRenderPass( DebugRenderPass && ) = delete;
        DebugRenderPass &operator=( const DebugRenderPass & ) = delete;
        DebugRenderPass &operator=( DebugRenderPass && ) = delete;

        void OnRender( Framebuffer::Ref framebuffer,
            const Camera &camera,
            const DirectX::XMFLOAT4X4 &cameraTransform ) override;
    };
}