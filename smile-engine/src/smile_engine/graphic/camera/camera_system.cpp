/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "camera_system.h"

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/scene/components.h"

namespace smile::graphic
{
    void CameraSystem::OnUpdate( Timestep deltaTime )
    {
        Camera *pMainCamera = nullptr;
        DirectX::XMFLOAT4X4 cameraTransform{};
        {
            auto view = m_pECSEngine->GetView< scene::TransformComponent, scene::CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] =
                    m_pECSEngine->GetComponents< scene::TransformComponent, scene::CameraComponent >( entity );

                if ( camera.IsPrimary )
                {
                    pMainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        RenderEngine::SetCameraData( { pMainCamera, cameraTransform } );
    }
}