/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "camera_system.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/renderer/render_engine.h"
#include "smile_engine/core/scene/components.h"

namespace smile::graphic::ecs
{
    void CameraSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        ecsEngine.RegisterComponentIfNeeded< scene::ecs::TransformComponent >();
        ecsEngine.RegisterComponentIfNeeded< CameraComponent >();
        System::OnAdd( ecsEngine );
    }

    void CameraSystem::OnUpdate()
    {
        Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform{};
        {
            auto view = m_pECSEngine->GetView< scene::ecs::TransformComponent, graphic::ecs::CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] =
                    m_pECSEngine->GetComponents< scene::ecs::TransformComponent, CameraComponent >( entity );

                if ( camera.IsPrimary )
                {
                    pMainCamera = &camera.Camera;
                    cameraTransform = transform.GetWorldTransform();
                    break;
                }
            }
        }

        RenderEngine::SetCameraData( { pMainCamera, cameraTransform } );
    }
}