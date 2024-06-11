/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/common/compiled/singleton.h"

#include "render_collector.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/camera/editor_camera.h"

#include "smile_engine/graphic/resource/rasterizer_state.h"
#include "smile_engine/core/scene/components.h"

namespace smile::graphic
{
    class WireframeRenderer final : public compiled::Singleton< WireframeRenderer >
    {
      public:
        void Initialize();
        void ShutDown();

        void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        void BeginScene( const EditorCamera &editorCamera );
        void EndScene();
        void OnRender();

        void Submit( const scene::BoxColliderComponent &boxColliderComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        void Submit( const scene::SphereColliderComponent &sphereColliderComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        void ClearDrawList();

      private:
        RenderCollector m_RenderCollector{};
        Ref< RasterizerState > s_pWireframeRasterizerState;
    };
}