#pragma once

#include "render_collector.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/camera/editor_camera.h"

#include "smile_engine/scene/components.h"

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
        static void Submit( const scene::MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const scene::StaticMeshComponent &staticMeshComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const scene::SkinnedMeshComponent &skinnedMeshComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

      private:
        static void ClearDrawlist();

      private:
        static RenderCollector *s_pRenderCollector;
    };
}
