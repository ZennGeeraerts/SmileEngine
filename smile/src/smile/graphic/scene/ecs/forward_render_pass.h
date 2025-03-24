/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "render_pass.h"

namespace smile::graphic::ecs
{
    class ForwardRenderPass final : public RenderPass
    {
      public:
        ForwardRenderPass() = default;

        ForwardRenderPass( const ForwardRenderPass & ) = delete;
        ForwardRenderPass( ForwardRenderPass && ) = delete;
        ForwardRenderPass &operator=( const ForwardRenderPass & ) = delete;
        ForwardRenderPass &operator=( ForwardRenderPass && ) = delete;

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) override;
    };
}