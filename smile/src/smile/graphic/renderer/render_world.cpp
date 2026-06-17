#include "smpch.h"
#include "render_world.h"

#include "smile/core/world/ecs/id_component.h"
#include "smile/core/world/ecs/transform_component.h"
#include "sprite/ecs/sprite_renderer_component.h"
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
        m_EntityMap[uuid] = handle;

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

                ViewConstants viewCons{};
                view.FillConstants( viewCons );

                view.OnUpdate();

                AddOrReplaceComponent< View >( entity, view );

                ConstantBufferDescriptor cbDesc{
                    { { "ViewProjection", ConstantType::Mat4, 1 }, { "ViewInverse", ConstantType::Mat4, 1 } } };

                ConstantBuffer cameraConstantBuffer = resourceManager.GetOrCreateConstantBuffer( cbDesc );

                cameraConstantBuffer.Update( &viewCons );
                renderContext.FillConstantBuffer( cameraConstantBuffer );
                AddOrReplaceComponent< ConstantBuffer >( entity, cameraConstantBuffer );

                const rhi::BindingSetDescriptor bindingSetDesc{
                    { rhi::BindingSetElement::CreateConstantBuffer( 0, cameraConstantBuffer.GetHandle() ) } };

                const BindingSet bindingSet = resourceManager.GetOrCreateBindingSet(
                    bindingSetDesc, viewBindingLayout, { rhi::ShaderStage::Vertex } );

                AddOrReplaceComponent< BindingSet >( entity, bindingSet );

                m_OpaqueCommandBuffers[entity] = DrawCommandBuffer{};
            }
        }

        // {
        //     auto group = m_ECSEngine.GetGroup< ecs::SpriteRendererComponent >(
        //         smile::ecs::g_Get< world::ecs::TransformComponent > );

        //     for ( auto entity : group )
        //     {
        //         const auto &[spriteRenderer, transform] =
        //             m_ECSEngine.GetComponents< SpriteRendererComponent, world::ecs::TransformComponent >( entity
        //             );

        //         auto materialInstance = materialSystem.GetOrCreateMaterialInstance( spriteRenderer.Material );
        //         auto texture = resourceManager.GetOrCreateTexture2D( spriteRenderer.Texture );

        //         materialInstance.SetParameter( "Color", spriteRenderer.Color );
        //         materialInstance.SetTextureBinding( "Texture", texture, rhi::SamplerDescriptor{} );

        //         spriteBatch.Submit( transform.GetWorldMatrix(), materialInstance );
        //     }
        // }

        {
            auto group = m_ECSEngine.GetGroup< ecs::MeshRendererComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< ecs::MeshRendererComponent, world::ecs::TransformComponent >( entity );

                const auto mesh = meshManager.GetOrCreateMesh( meshRenderer.Mesh );
                const auto materialInstance = materialSystem.GetOrCreateMaterialInstance( meshRenderer.Material );
                materialSystem.UpdateMaterialInstance( materialInstance );

                AddOrReplaceComponent< Mesh >( entity, mesh );
                AddOrReplaceComponent< MaterialInstance >( entity, materialInstance );

                const auto &materialData = materialSystem.GetMaterialData( materialInstance );
                const auto material = materialInstance.GetMaterial();

                GraphicsPipelineDescriptor psoDesc{};
                psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
                psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

                psoDesc.VertexShader =
                    resourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );
                psoDesc.PixelShader =
                    resourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

                psoDesc.BindingLayouts.PushBack( viewBindingLayout );
                psoDesc.BindingLayouts.PushBack( materialData.BindingLayout );

                psoDesc.RenderState = material.GetLayout().RenderState;

                const auto pipeline = resourceManager.GetOrCreateGraphicsPipeline( psoDesc );

                AddOrReplaceComponent< GraphicsPipeline >( entity, pipeline );
            }
        }
    }

    const DrawCommandBuffer &RenderWorld::GetOpaqueCommandBuffer( const smile::ecs::EntityHandle entity ) const
    {
        return m_OpaqueCommandBuffers[entity];
    }

    void RenderWorld::Enqueue()
    {
        auto viewGroup = m_ECSEngine.GetGroup< View >();

        for ( auto viewEntity : viewGroup )
        {
            const auto &view = m_ECSEngine.GetComponent< View >( viewEntity );
            const auto &viewMatrix = view.GetViewMatrix();

            auto group = m_ECSEngine.GetGroup< GraphicsPipeline, MaterialInstance, Mesh >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[pipeline, materialInstance, mesh, transform] =
                    m_ECSEngine
                        .GetComponents< GraphicsPipeline, MaterialInstance, Mesh, world::ecs::TransformComponent >(
                            entity );

                const BinKey binKey = bin_key::EncodeBin(
                    pipeline.GetHandle(), materialInstance.GetHandle().GetIndex(), mesh.Handle.GetIndex() );

                const float depth = transform.WorldTranslation.x * viewMatrix._13 +
                                    transform.WorldTranslation.y * viewMatrix._23 +
                                    transform.WorldTranslation.z * viewMatrix._33 + viewMatrix._43;

                const SortKey sortKey = sort_key::EncodeOpaque(
                    pipeline.GetHandle().GetIndex(), materialInstance.GetHandle().GetIndex(), depth );

                m_OpaqueCommandBuffers[viewEntity].Add( binKey, sortKey, entity );
            }

            m_OpaqueCommandBuffers[viewEntity].Sort();
        }
    }
}