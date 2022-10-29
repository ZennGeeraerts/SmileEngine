#pragma once
#include "perspective_camera.h"
#include "smile_engine/core/timestep.h"

#include "smile_engine/core/events/application_event.h"
#include "smile_engine/core/events/mouse_event.h"

#include "smile_engine/scene/components.h"

namespace Smile::Graphic
{
    class PerspectiveCameraController final
    {
     public:
        PerspectiveCameraController( float fov, float aspectRatio );

        void OnUpdate( Timestep deltaTime );
        void OnEvent( Event &e );

     private:
        bool OnMouseScrolled( MouseScrolledEvent &e );
        bool OnWindowResizedEvent( WindowResizeEvent &e );

     private:
        float m_FOV = 45.f;
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;

        PerspectiveCamera m_Camera;

        DirectX::XMFLOAT3 m_CameraPosition = { 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 m_CameraRotation = { 0.f, 0.f, 0.f };
        float m_CameraMoveSpeed = m_ZoomLevel;
        float m_CameraRotationSpeed = 180.f;
    };
}
