/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"

namespace smile::graphic::ecs
{
    class WireframeRenderPass final : public RenderPass
    {
      public:
        WireframeRenderPass( smile::ecs::ECSEngine &ecsEngine ) : RenderPass{ ecsEngine }
        {
        }

        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) override;
    };
}