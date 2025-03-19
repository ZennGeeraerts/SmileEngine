/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "camera_system.h"

#include "smile/graphic/camera/camera.h"
#include "smile/graphic/renderer/render_engine.h"
#include "smile/core/world/components.h"
#include "smile/core/world/world_manager.h"
#include "smile/core/world/entity.h"

namespace smile::graphic::ecs
{
    void CameraSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        ecsEngine.RegisterComponentIfNeeded< world::ecs::TransformComponent >();
        ecsEngine.RegisterComponentIfNeeded< CameraComponent >();
        System::OnAdd( ecsEngine );

        auto onCameraAddedFunc = []( smile::ecs::ECSEngine &ecsEngine, smile::ecs::EntityHandle entity )
        {
            auto &cameraComponent = ecsEngine.GetComponent< CameraComponent >( entity );

            memory::Ref< Scene > pScene = RenderEngine::GetScene();
            const Uint32 viewportWidth = pScene->GetViewportWidth();
            const Uint32 viewportHeight = pScene->GetViewportHeight();

            if ( viewportWidth > 0 && viewportHeight > 0 )
                cameraComponent.Camera.SetViewportSize( viewportWidth, viewportHeight );
        };

        ecsEngine.OnConstruction< CameraComponent >().emplace_back( onCameraAddedFunc );
    }

    void CameraSystem::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        RenderEngine::GetScene()->SetPrimaryCameraEntity( world::Entity{} );
        ecsEngine.OnConstruction< CameraComponent >().clear();
        System::OnRemove( ecsEngine );
    }

    void CameraSystem::OnUpdate()
    {
        memory::Ref< Scene > pScene = RenderEngine::GetScene();
        const Uint32 viewportWidth = pScene->GetViewportWidth();
        const Uint32 viewportHeight = pScene->GetViewportHeight();

        bool primaryCameraEntityDirty = false;

        auto view = m_pECSEngine->GetView< graphic::ecs::CameraComponent >();
        for ( auto entity : view )
        {
            auto &camera = m_pECSEngine->GetComponent< CameraComponent >( entity );

            if ( camera.IsPrimary )
            {
                world::Entity primaryCameraEntity{ entity, world::WorldManager::GetActive().get() };
                pScene->SetPrimaryCameraEntity( primaryCameraEntity );
                primaryCameraEntityDirty = true;
            }

            if ( !camera.HasFixedAspectRatio )
            {
                camera.Camera.SetViewportSize( viewportWidth, viewportHeight );
            }
        }

        if ( !primaryCameraEntityDirty )
            pScene->SetPrimaryCameraEntity( world::Entity{} );
    }
}