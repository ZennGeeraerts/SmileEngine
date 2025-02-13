/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "foundation/singleton.h"

#include "render_collector.h"

#include "engine/graphic/camera/camera.h"
#include "engine/graphic/renderer_api/resource/rasterizer_state.h"

#include "world/components.h"

namespace smile::graphic
{
    class WireframeRenderer final : public foundation::Singleton< WireframeRenderer >
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
        Ref< RasterizerState > s_pWireframeRasterizerState;
    };
}