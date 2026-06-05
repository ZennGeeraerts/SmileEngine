#include "smpch.h"
#include "render_world.h"

#include "smile/core/world/ecs/id_component.h"
#include "smile/core/world/ecs/transform_component.h"
#include "sprite/ecs/sprite_renderer_component.h"
#include "mesh/mesh_manager.h"
#include "render_context.h"
#include "material/material_system.h"
#include "resource/resource_manager.h"
#include "scene/ecs/camera_component.h"
#include "view.h"
#include "draw/draw_command_buffer.h"
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
        m_ECSEngine.AddComponent< ecs::IDComponent >( handle, uuid );
        m_EntityMap[uuid] = handle;

        return handle;
    }

    void RenderWorld::Prepare( RenderContext &renderContext,
        ResourceManager &resourceManager,
        MeshManager &meshManager,
        MaterialSystem &materialSystem,
        View &view )
    {
        view.SetViewport( m_ViewportState.Viewports[0] );

        BindingLayout bindingLayout;
        BindingSet bindingSet;

        {
            auto group = m_ECSEngine.GetGroup< ecs::CameraComponent, world::ecs::TransformComponent >();

            for ( auto entity : group )
            {
                const auto &[camera, transform] =
                    m_ECSEngine.GetComponents< ecs::CameraComponent, world::ecs::TransformComponent >( entity );

                if ( !camera.HasFixedAspectRatio )
                {
                    const Uint32 viewportWidth =
                        foundation::NumericCast< Uint32 >( m_ViewportState.Viewports[0].GetWidth() );
                    const Uint32 viewportHeight =
                        foundation::NumericCast< Uint32 >( m_ViewportState.Viewports[0].GetHeight() );

                    if ( viewportWidth > 0 && viewportHeight > 0 )
                        camera.Camera.SetViewportSize( viewportWidth, viewportHeight );
                }

                if ( camera.IsPrimary )
                {
                    const auto worldTransform = transform.GetWorldTransform();
                    auto viewMatrixMat =
                        DirectX::XMMatrixInverse( nullptr, DirectX::XMLoadFloat4x4( &worldTransform ) );
                    DirectX::XMFLOAT4X4 viewMatrix{};
                    DirectX::XMStoreFloat4x4( &viewMatrix, viewMatrixMat );

                    view.SetViewProjectionMatrix( viewMatrix, camera.Camera.GetProjectionMatrix() );

                    ViewConstants viewCons{};
                    view.FillConstants( viewCons );

                    ConstantBufferDescriptor cbDesc{
                        { "ViewProjection", ConstantType::Mat4 }, { "ViewInverse", ConstantType::Mat4 } };

                    const ConstantBuffer cameraConstantBuffer = resourceManager.GetOrCreateConstantBuffer( cbDesc );

                    cameraConstantBuffer.Update( &viewCons );
                    renderContext.FillConstantBuffer( cameraConstantBuffer );
                    AddOrReplaceComponent< ConstantBuffer >( entity, cameraConstantBuffer );

                    rhi::BindingSetDescriptor bindingSetDesc{
                        { rhi::BindingSetElement::CreateConstantBuffer( 0, cameraConstantBuffer.GetHandle() ) } };

                    resourceManager.GetOrCreateBindingSetAndLayout(
                        bindingSetDesc, { rhi::ShaderStage::Vertex }, bindingLayout, bindingSet );
                }
            }
        }

        // {
        //     auto group = m_ECSEngine.GetGroup< ecs::SpriteRendererComponent >(
        //         smile::ecs::g_Get< world::ecs::TransformComponent > );

        //     for ( auto entity : group )
        //     {
        //         const auto &[spriteRenderer, transform] =
        //             m_ECSEngine.GetComponents< SpriteRendererComponent, world::ecs::TransformComponent >( entity );

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

                AddOrReplaceComponent< Mesh >( entity, mesh );
                AddOrReplaceComponent< MaterialInstance >( entity, materialInstance );
                EnsurePipeline( materialInstance, bindingLayout, resourceManager, materialSystem );

                const auto pipelineIt = m_PipelineCache.FindItemAtKey( materialInstance );
                AddOrReplaceComponent< GraphicsPipeline >( entity, pipelineIt.GetItem() );
            }
        }
    }

    void RenderWorld::EnsurePipeline( const MaterialInstance &materialInstance,
        const BindingLayout &layout,
        ResourceManager &resourceManager,
        MaterialSystem &materialSystem )
    {
        if ( m_PipelineCache.FindItemAtKey( materialInstance ) != m_PipelineCache.end() )
            return;

        const auto &materialData = materialSystem.GetMaterialData( materialInstance );
        const auto material = materialInstance.GetMaterial();

        GraphicsPipelineDescriptor psoDesc{};
        psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
        psoDesc.InputLayout = materialData.ShaderProgram->GetVertexLayout();

        psoDesc.VertexShader = resourceManager.GetOrCreateVertexShader( materialData.ShaderProgram->GetVertexShader() );
        psoDesc.PixelShader = resourceManager.GetOrCreatePixelShader( materialData.ShaderProgram->GetPixelShader() );

        psoDesc.BindingLayouts.PushBack( layout );
        psoDesc.BindingLayouts.PushBack( materialData.BindingLayout );

        psoDesc.RenderState = material.GetLayout().RenderState;

        m_PipelineCache.Insert( materialInstance, resourceManager.CreateGraphicsPipeline( psoDesc ) );
    }

    void RenderWorld::Enqueue( DrawCommandBuffer &opaqueCommandBuffer )
    {
        auto group = m_ECSEngine.GetGroup< GraphicsPipeline, MaterialInstance, Mesh >();

        for ( auto entity : group )
        {
            const auto &[pipeline, materialInstance, mesh] =
                m_ECSEngine.GetComponents< GraphicsPipeline, MaterialInstance, Mesh >( entity );

            const SortKey key = sort_key::EncodeOpaque(
                pipeline.GetHandle().GetIndex(), materialInstance.GetHandle().GetIndex(), 0.0f );

            opaqueCommandBuffer.Add( key, entity );
        }
    }
}