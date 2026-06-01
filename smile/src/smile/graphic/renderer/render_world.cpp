#include "smpch.h"
#include "render_world.h"

#include "smile/core/world/ecs/id_component.h"
#include "smile/core/world/ecs/transform_component.h"
#include "sprite/ecs/sprite_renderer_component.h"
#include "sprite/sprite_batch.h"
#include "mesh/mesh_manager.h"
#include "material/material_system.h"
#include "resource/resource_manager.h"
#include "scene/ecs/camera_component.h"
#include "view.h"

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

    void RenderWorld::Prepare( ResourceManager &resourceManager,
        MeshManager &meshManager,
        MaterialSystem &materialSystem,
        View &view,
        SpriteBatch &spriteBatch )
    {
        view.SetViewport( m_ViewportState.Viewports[0] );

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
                }
            }
        }

        {
            auto group = m_ECSEngine.GetGroup< ecs::SpriteRendererComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[spriteRenderer, transform] =
                    m_ECSEngine.GetComponents< SpriteRendererComponent, world::ecs::TransformComponent >( entity );

                auto materialInstance = materialSystem.GetOrCreateMaterialInstance( spriteRenderer.Material );
                auto texture = resourceManager.GetOrCreateTexture2D( spriteRenderer.Texture );

                materialInstance.SetParameter( "Color", spriteRenderer.Color );
                materialInstance.SetTextureBinding( "Texture", texture, rhi::SamplerDescriptor{} );

                spriteBatch.Submit( transform.GetWorldMatrix(), materialInstance );
            }
        }

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
            }
        }
    }
}