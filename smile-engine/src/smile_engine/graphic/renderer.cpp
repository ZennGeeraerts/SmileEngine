#include "smpch.h"
#include "renderer.h"

namespace smile::graphic
{
    RendererSettings Renderer::settings{};
    RenderCollector *Renderer::renderCollector = new RenderCollector{};

    ShaderLibrary Renderer::shaderLibrary{};

    Ref< Framebuffer > Renderer::finalSceneFramebuffer{};

    Ref< RasterizerState > Renderer::wireframeRasterizerState{};

    void Renderer::initialize()
    {
        RenderCommand::initalize();

        DirectX::XMStoreFloat4x4( &renderCollector->viewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &renderCollector->viewProjectionMatrix, DirectX::XMMatrixIdentity() );

        BufferLayout buffer_layout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float3, "NORMAL" } };
        shaderLibrary.load( "assets/shaders/PosColNorm.fx", buffer_layout );
        shaderLibrary.load( "assets/shaders/PosCol.fx", { { ShaderDataType::Float3, "POSITION" } } );

        {
            FramebufferData frame_buffer_data{};
            frame_buffer_data.attachments = { { FramebufferTextureFormat::RGBA8, true },
                FramebufferTextureFormat::Depth,
                { FramebufferTextureFormat::RGBA8, true } };
            frame_buffer_data.width = settings.width;
            frame_buffer_data.height = settings.height;
            frame_buffer_data.swapChainTarget = false;

            finalSceneFramebuffer = Framebuffer::create( frame_buffer_data );
            finalSceneFramebuffer->setClearColor( { DirectX::Colors::DodgerBlue.f[0],
                DirectX::Colors::DodgerBlue.f[1],
                DirectX::Colors::DodgerBlue.f[2],
                DirectX::Colors::DodgerBlue.f[3] } );
        }
        {
            RasterizerStateData rasterizerStateData{};
            rasterizerStateData.cullMode = CullMode::None;
            rasterizerStateData.fillMode = FillMode::WireFrame;
            rasterizerStateData.depthClipEnable = true;

            wireframeRasterizerState = RasterizerState::create( rasterizerStateData );
        }
    }

    void Renderer::shutDown()
    {
        clearDrawlist();
        SAFE_DELETE( renderCollector );

        RenderCommand::shutDown();
    }

    void Renderer::setSettings( const RendererSettings &new_settings )
    {
        settings = new_settings;
    }

    void Renderer::onWindowResize( Uint32 width, Uint32 height )
    {
        RenderCommand::resizeWindow( 0, 0, width, height );
    }

    void Renderer::resizeFramebuffer( Uint32 width, Uint32 height )
    {
        settings.width = width;
        settings.height = height;

        finalSceneFramebuffer->resize( width, height );
    }

    void Renderer::beginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &camera_transform )
    {
        auto camera_transform_mat = DirectX::XMLoadFloat4x4( &camera_transform );
        auto projection_matrix_mat = DirectX::XMLoadFloat4x4( &camera.getProjectionMatrix() );
        auto view_matrix_mat = DirectX::XMMatrixInverse( nullptr, camera_transform_mat );
        auto view_projection_matrix_mat = view_matrix_mat * projection_matrix_mat;

        DirectX::XMStoreFloat4x4( &renderCollector->viewProjectionMatrix, view_projection_matrix_mat );
        DirectX::XMStoreFloat4x4( &renderCollector->viewInverseMatrix, camera_transform_mat );
    }

    void Renderer::beginScene( const EditorCamera &editor_camera )
    {
        renderCollector->viewProjectionMatrix = editor_camera.getViewProjectionMatrix();

        DirectX::XMFLOAT4X4 view_matrix = editor_camera.getViewMatrix();
        auto view_matrix_mat = DirectX::XMLoadFloat4x4( &view_matrix );
        DirectX::XMStoreFloat4x4(
            &renderCollector->viewInverseMatrix, DirectX::XMMatrixInverse( nullptr, view_matrix_mat ) );
    }

    void Renderer::submit( const Ref< VertexBuffer > &vertexBuffer,
        const Ref< IndexBuffer > &indexBuffer,
        const Ref< Shader > &shader,
        const DirectX::XMFLOAT4X4 &world_transform )
    {
        renderCollector->geometryDrawList.emplace_back(
            DrawCommand{ vertexBuffer, indexBuffer, shader, world_transform } );
    }

    void Renderer::submit( const scene::MeshRendererComponent &mesh_renderer_component,
        const DirectX::XMFLOAT4X4 &world_transform )
    {
        submit( mesh_renderer_component.vertexBuffer,
            mesh_renderer_component.indexBuffer,
            mesh_renderer_component.shader,
            world_transform );
    }

    void Renderer::submit( const scene::StaticMeshComponent &static_mesh_component, const DirectX::XMFLOAT4X4 &world_transform )
    {
        for ( const auto &mesh : static_mesh_component.meshes )
        {
            submit( mesh->getVertexBuffer(),
                mesh->getIndexBuffer(),
                static_mesh_component.materials[0]->getShader(),
                world_transform );
        }
    }

    void Renderer::submit( const scene::SkinnedMeshComponent &skinned_mesh_component, const DirectX::XMFLOAT4X4 &world_transform )
    {
        for ( const auto &mesh : skinned_mesh_component.meshes )
        {
            submit( mesh->getVertexBuffer(),
                mesh->getIndexBuffer(),
                skinned_mesh_component.materials[0]->getShader(),
                world_transform );
        }
    }

    void Renderer::submitWireframe( const scene::BoxColliderComponent &box_collider_component,
        const DirectX::XMFLOAT4X4 &world_transform )
    {
        DirectX::XMMATRIX final_transform_mat = DirectX::XMLoadFloat4x4( &world_transform );
        DirectX::XMVECTOR translation_vec{};
        DirectX::XMVECTOR rotation_vec{};
        DirectX::XMVECTOR scale_vec{};

        DirectX::XMMatrixDecompose( &scale_vec, &rotation_vec, &translation_vec, final_transform_mat );
        DirectX::XMVECTOR offset_vec = DirectX::XMLoadFloat3( &box_collider_component.offset );
        auto final_translation_vec = DirectX::XMVectorAdd( translation_vec, offset_vec );
        DirectX::XMVECTOR size_vec = DirectX::XMLoadFloat3( &box_collider_component.size );

        size_vec = DirectX::XMVectorDivide( size_vec, DirectX::XMVECTOR{ 2, 2, 2 } );

        auto final_scale_vec = DirectX::XMVectorMultiply( scale_vec, size_vec );
        final_transform_mat = DirectX::XMMatrixScalingFromVector( final_scale_vec ) *
                            DirectX::XMMatrixRotationQuaternion( rotation_vec ) *
                            DirectX::XMMatrixTranslationFromVector( final_translation_vec );

        DirectX::XMFLOAT4X4 final_transform{};
        DirectX::XMStoreFloat4x4( &final_transform, final_transform_mat );

        DrawCommand draw_command{ box_collider_component.wireframeMesh->getVertexBuffer(),
            box_collider_component.wireframeMesh->getIndexBuffer(),
            shaderLibrary.get( "PosCol" ),
            final_transform };
        renderCollector->wireframeDrawList.emplace_back( draw_command );
    }

    void Renderer::onRender()
    {
        finalSceneFramebuffer->clear();
        finalSceneFramebuffer->bind();

        for ( const DrawCommand &draw_command : renderCollector->geometryDrawList )
        {
            draw_command.vertexBuffer->bind();
            draw_command.indexBuffer->bind();
            draw_command.shader->bind();

            draw_command.shader->uploadMat4( "ViewProjection", renderCollector->viewProjectionMatrix );
            draw_command.shader->uploadMat4( "World", draw_command.worldTransform );
            draw_command.shader->uploadMat4( "ViewInverse", renderCollector->viewInverseMatrix );

            RenderCommand::drawIndexed( draw_command.indexBuffer->getCount(), draw_command.shader );
        }

        wireframeRasterizerState->bind();

        for ( const DrawCommand &drawCommand : renderCollector->wireframeDrawList )
        {
            drawCommand.vertexBuffer->bind();
            drawCommand.indexBuffer->bind();
            drawCommand.shader->bind();

            drawCommand.shader->uploadMat4( "ViewProjection", renderCollector->viewProjectionMatrix );
            drawCommand.shader->uploadMat4( "World", drawCommand.worldTransform );
            // drawCommand.pShader->UploadMat4("ViewInverse", m_pRenderCollector->ViewInverseMatrix);

            RenderCommand::drawIndexed( drawCommand.indexBuffer->getCount(), drawCommand.shader );
        }

        wireframeRasterizerState->unbind();

        finalSceneFramebuffer->unbind();
    }

    void Renderer::endScene()
    {
        clearDrawlist();
    }

    void Renderer::clearDrawlist()
    {
        renderCollector->geometryDrawList.clear();
        renderCollector->wireframeDrawList.clear();
    }
}