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

namespace smile::graphic
{
    struct RendererSettings final
    {
        Uint32 width = 1280;
        Uint32 height = 720;
    };

    struct DrawCommand final
    {
        Ref< VertexBuffer > vertexBuffer;
        Ref< IndexBuffer > indexBuffer;
        Ref< Shader > shader;
        DirectX::XMFLOAT4X4 worldTransform;
    };

    struct RenderCollector final
    {
        DirectX::XMFLOAT4X4 viewInverseMatrix;
        DirectX::XMFLOAT4X4 viewProjectionMatrix;

        std::vector< DrawCommand > geometryDrawList;
        std::vector< DrawCommand > wireframeDrawList;
    };

    class Renderer final
    {
      public:
        static void initialize();
        static void shutDown();

        static void setSettings( const RendererSettings &new_settings );

        static void onWindowResize( Uint32 width, Uint32 height );
        static void resizeFramebuffer( Uint32 width, Uint32 height );

        static void beginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &camera_transform );
        static void beginScene( const EditorCamera &editor_camera );
        static void endScene();
        static void onRender();

        static void submit( const Ref< VertexBuffer > &vertex_buffer,
            const Ref< IndexBuffer > &index_buffer,
            const Ref< Shader > &shader,
            const DirectX::XMFLOAT4X4 &world_transform );
        static void submit( const scene::MeshRendererComponent &mesh_renderer_component,
            const DirectX::XMFLOAT4X4 &world_transform );
        static void submit( const scene::StaticMeshComponent &static_mesh_component,
            const DirectX::XMFLOAT4X4 &world_transform );
        static void submit( const scene::SkinnedMeshComponent &skinned_mesh_component,
            const DirectX::XMFLOAT4X4 &world_transform );

        static void submitWireframe( const scene::BoxColliderComponent &box_collider_component,
            const DirectX::XMFLOAT4X4 &world_transform );

        static RendererAPI::API getAPI()
        {
            return RendererAPI::getAPI();
        }
        static const RendererSettings &getSettings()
        {
            return settings;
        }
        static void *getFinalColor()
        {
            return finalSceneFramebuffer->getColor( 0 );
        }

      private:
        static void clearDrawlist();

      private:
        static RendererSettings settings;
        static RenderCollector *renderCollector;

        static ShaderLibrary shaderLibrary;

        static Ref< Framebuffer > finalSceneFramebuffer;
        static Ref< RasterizerState > wireframeRasterizerState;
    };
}
