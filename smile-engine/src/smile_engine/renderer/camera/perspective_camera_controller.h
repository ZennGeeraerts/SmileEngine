#pragma once
#include "perspective_camera.h"
#include "smile_engine/core/timestep.h"

#include "smile_engine/core/events/application_event.h"
#include "smile_engine/core/events/mouse_event.h"

#include "smile_engine/scene/components.h"

namespace smile::renderer
{
    class PerspectiveCameraController final
    {
     public:
        PerspectiveCameraController( float fov, float aspect_ratio );

        void onUpdate( Timestep delta_time );
        void onEvent( Event &e );

     private:
        bool onMouseScrolled( MouseScrolledEvent &e );
        bool onWindowResizedEvent( WindowResizeEvent &e );

     private:
        float fov = 45.f;
        float aspectRatio;
        float zoomLevel = 1.0f;

        PerspectiveCamera camera;

        DirectX::XMFLOAT3 cameraPosition = { 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 cameraRotation = { 0.f, 0.f, 0.f };
        float cameraMoveSpeed = zoomLevel;
        float cameraRotationSpeed = 180.f;
    };
}
