#pragma once

#include "camera.h"
#include "smile_engine/core/events/event.h"
#include "smile_engine/core/events/mouse_event.h"

namespace smile::graphic
{
    class EditorCamera final : public Camera
    {
      public:
        EditorCamera() = default;
        EditorCamera( float fov, float aspect_ratio, float near_plane, float far_plane );

        void onUpdate( Timestep delta_time );
        void onEvent( Event &e );

        float getDistance() const
        {
            return distance;
        }
        void setDistance( float distance )
        {
            distance = distance;
        }

        void setViewportSize( float width, float height )
        {
            viewportWidth = width;
            viewportHeight = height;
            updateProjection();
        }

        const DirectX::XMFLOAT4X4 &getViewMatrix() const
        {
            return viewMatrix;
        }
        DirectX::XMFLOAT4X4 getViewProjectionMatrix() const;

        DirectX::XMFLOAT3 getUpDirection() const;
        DirectX::XMFLOAT3 getRightDirection() const;
        DirectX::XMFLOAT3 getForwardDirection() const;
        const DirectX::XMFLOAT3 &getPosition() const
        {
            return position;
        }

        float getPitch() const
        {
            return pitch;
        }
        float getYaw() const
        {
            return yaw;
        }

      private:
        void updateView();
        void updateProjection();

        bool onMouseScroll( MouseScrolledEvent &e );

        void mousePan( const DirectX::XMFLOAT2 &delta );
        void mouseRotate( const DirectX::XMFLOAT2 &delta );
        void mouseZoom( float delta );

        DirectX::XMFLOAT3 calculatePosition() const;

        DirectX::XMFLOAT2 calculatePanSpeed() const;
        float calculateRotationSpeed() const;
        float calculateZoomSpeed() const;

      private:
        float fov = 45.f;
        float aspectRatio = 1.778f;
        float nearPlane = 0.1f;
        float farPlane = 2500.f;

        DirectX::XMFLOAT4X4 viewMatrix{};
        DirectX::XMFLOAT3 position{};
        DirectX::XMFLOAT3 focalPoint{};

        DirectX::XMFLOAT2 initialMousePosition{};

        float distance = 10.f;
        float pitch = 0.f;
        float yaw = 0.f;

        float viewportWidth = 1280.f;
        float viewportHeight = 720.f;
    };
}
