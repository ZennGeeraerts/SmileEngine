/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "render_pass.h"

namespace smile::graphic::ecs
{
    class RenderPass2D : public RenderPass
    {
      public:
        RenderPass2D() = default;

        RenderPass2D( const RenderPass2D & ) = delete;
        RenderPass2D( RenderPass2D && ) = delete;
        RenderPass2D &operator=( const RenderPass2D & ) = delete;
        RenderPass2D &operator=( RenderPass2D && ) = delete;

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) override;
    };
}