#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "RasterizerState.h"

#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"

#include "SmileEngine/Scene/Components.h"

namespace smile
{
    struct RendererSettings final
    {
        Uint32 m_Width = 1280;
        Uint32 m_Height = 720;
    };

    struct DrawCommand final
    {
        Ref< VertexBuffer > m_pVertexBuffer;
        Ref< IndexBuffer > m_pIndexBuffer;
        Ref< Shader > m_pShader;
        DirectX::XMFLOAT4X4 m_WorldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 m_ViewInverseMatrix;
        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        std::vector< DrawCommand > m_GeometryDrawList;
        std::vector< DrawCommand > m_WireframeDrawList;
    };

    class Renderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void SetSettings( const RendererSettings &settings );

        static void OnWindowResize( uint32_t width, uint32_t height );
        static void ResizeFramebuffer( uint32_t width, uint32_t height );

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void BeginScene( const EditorCamera &editorCamera );
        static void EndScene();
        static void OnRender();

        static void Submit( const Ref< VertexBuffer > &pVertexBuffer,
            const Ref< IndexBuffer > &pIndexBuffer,
            const Ref< Shader > &pShader,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const MeshRendererComponent &meshRendererComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const StaticMeshComponent &staticMeshComponent, const DirectX::XMFLOAT4X4 &worldTransform );
        static void Submit( const SkinnedMeshComponent &skinnedMeshComponent,
            const DirectX::XMFLOAT4X4 &worldTransform );

        static void SubmitWireframe( const BoxColliderComponent &boxColliderComponent,
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
