/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_collector.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/camera/editor_camera.h"

#include "smile_engine/graphic/mesh/ecs/mesh_renderer_component.h"
#include "smile_engine/graphic/mesh/ecs/skinned_mesh_renderer_component.h"

namespace smile::graphic
{
    class ForwardRenderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void BeginScene( const EditorCamera &editorCamera );
        static void EndScene();
        static void OnRender();

        static void Submit( const Ref< VertexBuffer > &pVertexBuffer,
            const Ref< IndexBuffer > &pIndexBuffer,
            const Ref< Shader > &pShader,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const ecs::MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const ecs::SkinnedMeshRendererComponent &skinnedMeshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        static void ClearDrawList();

      private:
        static RenderCollector s_RenderCollector;
    };
}
