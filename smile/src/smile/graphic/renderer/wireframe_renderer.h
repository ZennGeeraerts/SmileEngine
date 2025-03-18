/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/foundation/meyers_singleton.h"

#include "render_collector.h"

#include "smile/graphic/camera/camera.h"

#include "smile/core/world/components.h"

namespace smile::graphic
{
    class WireframeRenderer final : public foundation::MeyersSingleton< WireframeRenderer >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void EndScene();
        void OnRender();

        void Submit( const physics::ecs::BoxColliderComponent &boxColliderComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        void Submit( const physics::ecs::SphereColliderComponent &sphereColliderComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        void ClearDrawList();

      private:
        RenderCollector m_RenderCollector{};
        RenderState m_State;
    };
}