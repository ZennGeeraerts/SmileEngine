/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_collector.h"

#include "camera.h"

#include "smile/graphic/mesh/ecs/mesh_renderer_component.h"
#include "smile/graphic/mesh/ecs/skinned_mesh_renderer_component.h"

namespace smile::graphic
{
    class ForwardRenderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void EndScene();
        static void OnRender();

        static void Submit( const memory::Ref< VertexBuffer > &pVertexBuffer,
            const memory::Ref< IndexBuffer > &pIndexBuffer,
            const memory::Ref< Shader > &pShader,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const ecs::MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const ecs::SkinnedMeshRendererComponent &skinnedMeshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        static void ClearDrawList();

      private:
        static RenderCollector s_RenderCollector;
        static RenderState s_State;
    };
}
