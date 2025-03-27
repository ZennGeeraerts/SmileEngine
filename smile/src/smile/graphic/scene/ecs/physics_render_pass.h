/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_pass.h"

namespace smile::graphic::ecs
{
    class PhysicsRenderPass final : public RenderPass
    {
      public:
        PhysicsRenderPass() = default;

        PhysicsRenderPass( const PhysicsRenderPass & ) = delete;
        PhysicsRenderPass( PhysicsRenderPass && ) = delete;
        PhysicsRenderPass &operator=( const PhysicsRenderPass & ) = delete;
        PhysicsRenderPass &operator=( PhysicsRenderPass && ) = delete;

#if __has_include( <PxPhysicsAPI.h> )
        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) override;
#else
        void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform ) override
        {
        }
#endif
    };
}