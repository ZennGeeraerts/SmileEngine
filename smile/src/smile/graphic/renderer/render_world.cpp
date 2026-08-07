#include "smpch.h"
#include "render_world.h"

#include "smile/core/world/ecs/id_component.h"
#include "smile/core/world/ecs/transform_component.h"
#include "sprite/ecs/sprite_renderer_component.h"
#include "sprite/sprite_data.h"
#include "mesh/mesh_manager.h"
#include "mesh/ecs/mesh_renderer_component.h"
#include "render_context.h"
#include "material/material_system.h"
#include "resource/resource_manager.h"
#include "scene/ecs/camera_component.h"
#include "view.h"
#include "draw/sort_key.h"

namespace smile::graphic
{
    smile::ecs::EntityHandle RenderWorld::CreateEntity()
    {
        return CreateEntity( primitive::UUID{} );
    }

    smile::ecs::EntityHandle RenderWorld::CreateEntity( const primitive::UUID uuid )
    {
        const auto handle = m_ECSEngine.CreateEntity();
        m_ECSEngine.AddComponent< world::ecs::IDComponent >( handle, uuid );
        m_ECSEngine.AddComponent< world::ecs::TransformComponent >( handle );

        return handle;
    }

    void RenderWorld::Prepare( RenderContext &renderContext,
        ResourceManager &resourceManager,
        MeshManager &meshManager,
        MaterialSystem &materialSystem )
    {
        const rhi::BindingLayout bindingLayoutDesc{
            { rhi::ShaderStage::Vertex }, { { 0, rhi::ResourceType::ConstantBuffer } } };

        const BindingLayout viewBindingLayout = resourceManager.GetOrCreateBindingLayout( bindingLayoutDesc );

        {
            auto group = m_ECSEngine.GetGroup< ecs::CameraComponent, world::ecs::TransformComponent >();

            for ( auto entity : group )
            {
                const auto &[camera, transform] =
                    m_ECSEngine.GetComponents< ecs::CameraComponent, world::ecs::TransformComponent >( entity );

                SM_ASSERT( camera.RenderTarget.IsValid() );

                if ( !camera.HasFixedAspectRatio )
                {
                    const rhi::Viewport vp = camera.RenderTarget.GetViewport();
                    const Uint32 viewportWidth = static_cast< Uint32 >( vp.GetWidth() );
                    const Uint32 viewportHeight = static_cast< Uint32 >( vp.GetHeight() );

                    if ( viewportWidth > 0 && viewportHeight > 0 )
                        camera.Camera.SetViewportSize( viewportWidth, viewportHeight );
                }

                const auto worldTransform = transform.GetWorldTransform();
                auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, DirectX::XMLoadFloat4x4( &worldTransform ) );
                DirectX::XMFLOAT4X4 viewMatrix{};
                DirectX::XMStoreFloat4x4( &viewMatrix, viewMatrixMat );

                View view{};
                view.SetViewport( camera.RenderTarget.GetViewport() );
                view.SetViewProjectionMatrix( viewMatrix, camera.Camera.GetProjectionMatrix() );
                view.SetRenderTarget( camera.RenderTarget );

                view.OnUpdate();

                ViewConstants viewCons{};
                view.FillConstants( viewCons );

                AddOrReplaceComponent< View >( entity, view );

                const ConstantBufferDescriptor cbDesc{
                    { { "ViewProjection", ConstantType::Mat4, 1 }, { "ViewInverse", ConstantType::Mat4, 1 } } };

                ConstantBuffer cameraConstantBuffer = resourceManager.GetOrCreateConstantBuffer( cbDesc );

                cameraConstantBuffer.Update( &viewCons );
                renderContext.FillConstantBuffer( cameraConstantBuffer );
                AddOrReplaceComponent< ConstantBuffer >( entity, cameraConstantBuffer );

                const rhi::BindingSetDescriptor bindingSetDesc{
                    { rhi::BindingSetElement::CreateConstantBuffer( 0, cameraConstantBuffer.GetHandle() ) } };

                const BindingSet bindingSet = resourceManager.GetOrCreateBindingSet(
                    bindingSetDesc, viewBindingLayout, { rhi::ShaderStage::Vertex } );

                AddOrReplaceComponent< BindingLayout >( entity, viewBindingLayout );
                AddOrReplaceComponent< BindingSet >( entity, bindingSet );
            }
        }

        // auto &context = m_ECSEngine.GetContext();
        // if ( !context.Has< SpriteData >() )
        // {
        //     auto &spriteData = context.Emplace< SpriteData >();

        //     spriteData.VertexLayout =
        //         rhi::BufferLayout{ { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORDS" }
        //         };

        //     const Count quadVerticesCount = 12;
        //     float quadVertices[] = { -0.5f,
        //         -0.5f,
        //         0.0f,
        //         0.0f,
        //         1.0f,
        //         /*1*/ -0.5f,
        //         0.5f,
        //         0.0f,
        //         0.0f,
        //         0.0f,
        //         /*2*/ 0.5f,
        //         -0.5f,
        //         0.0f,
        //         1.0f,
        //         1.0f /*3*/,
        //         0.5f,
        //         0.5f,
        //         0.0f,
        //         1.0f,
        //         0.0f /*4*/ };

        //     spriteData.QuadVertexBuffer =
        //         resourceManager.CreateVertexBuffer( quadVertices, quadVerticesCount, spriteData.VertexLayout );

        //     const Count quadIndicesCount = 6;
        //     Uint32 quadIndices[] = { 0, 1, 2, 2, 1, 3 };
        //     spriteData.QuadIndexBuffer = resourceManager.CreateIndexBuffer( quadIndices, quadIndicesCount );
        // }

        // {
        //     auto group = m_ECSEngine.GetGroup< ecs::SpriteRendererComponent >(
        //         smile::ecs::g_Get< world::ecs::TransformComponent > );

        //     for ( auto entity : group )
        //     {
        //         const auto &[spriteRenderer, transform] =
        //             m_ECSEngine.GetComponents< ecs::SpriteRendererComponent, world::ecs::TransformComponent >( entity
        //             );

        //         spriteRenderer.Material->SetParameter( "Color", spriteRenderer.Color );
        //         spriteRenderer.Material->SetTextureBinding( "UseTexture", spriteRenderer.Texture, {} );

        //         const auto &spriteData = m_ECSEngine.GetContext().Get< SpriteData >();

        //         PrepareMaterial( entity, spriteRenderer.Material, materialSystem );

        //         Mesh spriteMesh{};
        //         spriteMesh.Handle = MeshHandle::NullHandle();
        //         spriteMesh.VertexBuffer = spriteData.QuadVertexBuffer;
        //         spriteMesh.IndexBuffer = spriteData.QuadIndexBuffer;

        //         AddOrReplaceComponent< Mesh >( entity, spriteMesh );

        //         PreparePipeline( entity,
        //             spriteData.VertexLayout,
        //             viewBindingLayout,
        //             spriteRenderer.Material->GetMaterial(),
        //             materialSystem.GetMaterialData( spriteRenderer.Material->GetMaterialInstance() ),
        //             resourceManager );
        //     }
        // }

        {
            auto group = m_ECSEngine.GetGroup< ecs::MeshRendererComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< ecs::MeshRendererComponent, world::ecs::TransformComponent >( entity );

                PrepareRenderable( entity,
                    viewBindingLayout,
                    meshRenderer.Mesh,
                    meshRenderer.Material,
                    resourceManager,
                    meshManager,
                    materialSystem );
            }
        }
    }

    void RenderWorld::PrepareRenderable( smile::ecs::EntityHandle entity,
        const BindingLayout &viewBindingLayout,
        const MeshSource::Ref &meshSource,
        const MaterialInstanceAsset::ConstRef &materialInstanceAsset,
        ResourceManager &resourceManager,
        MeshManager &meshManager,
        MaterialSystem &materialSystem )
    {
        const auto &materialInstance = PrepareMaterial( entity, materialInstanceAsset, materialSystem );

        const auto &materialData = materialSystem.GetMaterialData( materialInstance );
        const auto &vertexLayout = materialData.ShaderProgram->GetVertexLayout();

        PrepareMesh( entity, meshSource, vertexLayout, meshManager );

        PreparePipeline(
            entity, vertexLayout, viewBindingLayout, materialInstance.GetMaterial(), materialData, resourceManager );
    }

    const MaterialInstance &RenderWorld::PrepareMaterial( smile::ecs::EntityHandle entity,
        const MaterialInstanceAsset::ConstRef &materialInstanceAsset,
        MaterialSystem &materialSystem )
    {
        const auto materialInstance = materialSystem.GetOrCreateMaterialInstance( materialInstanceAsset );
        materialSystem.UpdateMaterialInstance( materialInstance );

        return AddOrReplaceComponent< MaterialInstance >( entity, materialInstance );
    }

    void RenderWorld::PrepareMesh( smile::ecs::EntityHandle entity,
        const MeshSource::Ref &meshSource,
        const rhi::BufferLayout &vertexLayout,
        MeshManager &meshManager )
    {
        const auto meshHandle = meshManager.CreateMeshIfNotExists( meshSource, vertexLayout );

        AddOrReplaceComponent< MeshHandle >( entity, meshHandle );
    }

    void RenderWorld::PreparePipeline( smile::ecs::EntityHandle entity,
        const rhi::BufferLayout &vertexLayout,
        const BindingLayout &viewBindingLayout,
        const Material &material,
        const MaterialData &materialData,
        ResourceManager &resourceManager )
    {
        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = vertexLayout;

        psoDesc.VertexShader = resourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );
        psoDesc.PixelShader = resourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        psoDesc.BindingLayouts.PushBack( viewBindingLayout );
        psoDesc.BindingLayouts.PushBack( materialData.BindingLayout );

        psoDesc.RenderState = material.GetLayout().RenderState;

        const auto psoDescHandle = resourceManager.GetOrCreateGraphicsPipelineDescriptor( psoDesc );

        AddOrReplaceComponent< GraphicsPipelineDescriptorHandle >( entity, psoDescHandle );
    }

    void RenderWorld::Enqueue( ViewBinnedCommandBuffers< Opaque3d > &opaque3dCommandBuffers,
        ViewSortedCommandBuffers< Transparent3d > &transparent3dCommandBuffers )
    {
        auto viewGroup = m_ECSEngine.GetGroup< View >();

        for ( auto viewEntity : viewGroup )
        {
            const auto &view = m_ECSEngine.GetComponent< View >( viewEntity );
            const auto &viewMatrix = view.GetViewMatrix();

            auto group = m_ECSEngine.GetGroup< GraphicsPipelineDescriptorHandle, MaterialInstance, MeshHandle >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[psoDescHandle, materialInstance, meshHandle, transform] =
                    m_ECSEngine.GetComponents< GraphicsPipelineDescriptorHandle,
                        MaterialInstance,
                        MeshHandle,
                        world::ecs::TransformComponent >( entity );

                const Opaque3dBinKey binKey{
                    psoDescHandle.GetIndex(), materialInstance.GetHandle().GetIndex(), meshHandle.GetIndex() };

                const float depth = transform.WorldTranslation.x * viewMatrix._13 +
                                    transform.WorldTranslation.y * viewMatrix._23 +
                                    transform.WorldTranslation.z * viewMatrix._33 + viewMatrix._43;

                const Opaque3d item{ entity, psoDescHandle, materialInstance, depth };

                opaque3dCommandBuffers[viewEntity].Add( binKey, item );
            }

            opaque3dCommandBuffers[viewEntity].Sort();
            transparent3dCommandBuffers[viewEntity].Sort();
        }
    }
}