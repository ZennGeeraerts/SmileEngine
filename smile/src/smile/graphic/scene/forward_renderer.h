/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/render_collector.h"
#include "smile/graphic/renderer/camera.h"

#include "ecs/mesh_renderer_component.h"
#include "ecs/skinned_mesh_renderer_component.h"

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

        static void Submit( const VertexBuffer::Ref &pVertexBuffer,
            const IndexBuffer::Ref &pIndexBuffer,
            const VertexShader::Ref &pVertexShader,
            const PixelShader::Ref &pPixelShader,
            const DirectX::XMFLOAT4X4 &worldTransform );

        static void Submit( const ecs::MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

        static void Submit( const ecs::SkinnedMeshRendererComponent &skinnedMeshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        static void ClearDrawList();

      private:
        static RenderCollector s_RenderCollector;
        static rhi::RenderState s_State;
    };
}
