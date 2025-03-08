/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "camera_system.h"

#include "engine/graphic/camera/camera.h"
#include "engine/graphic/renderer/render_engine.h"
#include "world/components.h"

namespace smile::graphic::ecs
{
    void CameraSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        ecsEngine.RegisterComponentIfNeeded< world::ecs::TransformComponent >();
        ecsEngine.RegisterComponentIfNeeded< CameraComponent >();
        System::OnAdd( ecsEngine );
    }

    void CameraSystem::OnUpdate()
    {
        Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform = [&]()
        {
            auto view = m_pECSEngine->GetView< world::ecs::TransformComponent, graphic::ecs::CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] =
                    m_pECSEngine->GetComponents< world::ecs::TransformComponent, CameraComponent >( entity );

                if ( camera.IsPrimary )
                {
                    pMainCamera = &camera.Camera;
                    return transform.GetWorldTransform();
                }
            }

            return DirectX::XMFLOAT4X4{};
        }();

        RenderEngine::SetCameraData( { pMainCamera, cameraTransform } );
    }
}