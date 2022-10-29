#pragma once

#include "render_command.h"
#include "shader/buffer.h"
#include "shader/shader.h"
#include "shader/shader_library.h"
#include "resource/frame_buffer.h"
#include "resource/rasterizer_state.h"

#include "camera/camera.h"
#include "camera/editor_camera.h"

#include "smile_engine/scene/components.h"

namespace Smile::Graphic
{
    struct RendererSettings final
    {
        Uint32 Width = 1280;
        Uint32 Height = 720;
    };

    struct DrawCommand final
    {
        Ref< VertexBuffer > pVertexBuffer;
        Ref< IndexBuffer > pIndexBuffer;
        Ref< Shader > pShader;
        DirectX::XMFLOAT4X4 WorldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 ViewInverseMatrix;
        DirectX::XMFLOAT4X4 ViewProjectionMatrix;

        std::vector< DrawCommand > GeometryDrawList;
        std::vector< DrawCommand > WireframeDrawList;
    };

    class Renderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void SetSettings( const RendererSettings &settings );

        static void OnWindowResize( Uint32 width, Uint32 height );
        static void ResizeFramebuffer( Uint32 width, Uint32 height );

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void BeginScene( const EditorCamera &editorCamera );
        static void EndScene();
        static void OnRender();

        static void Submit( const Ref< VertexBuffer > &pVertexBuffer,
            const Ref< IndexBuffer > &pIndexBuffer,
            const Ref< Shader > &pShader,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const Scene::MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const Scene::StaticMeshComponent &staticMeshComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const Scene::SkinnedMeshComponent &skinnedMeshComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

        static void SubmitWireframe( const Scene::BoxColliderComponent &boxColliderComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

        static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }
        static const RendererSettings &GetSettings()
        {
            return s_Settings;
        }
        static void *GetFinalColor()
        {
            return s_pFinalSceneFramebuffer->GetColor( 0 );
        }

      private:
        static void ClearDrawlist();

      private:
        static RendererSettings s_Settings;
        static RenderCollector *s_pRenderCollector;

        static ShaderLibrary s_ShaderLibrary;

        static Ref< Framebuffer > s_pFinalSceneFramebuffer;
        static Ref< RasterizerState > s_pWireframeRasterizerState;
    };
}
