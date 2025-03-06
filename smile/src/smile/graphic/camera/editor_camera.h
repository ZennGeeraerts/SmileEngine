/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "camera.h"
#include "smile/core/window/events/event.h"
#include "smile/core/window/events/mouse_event.h"

namespace smile::graphic
{
    class EditorCamera final : public Camera
    {
      public:
        EditorCamera() = default;
        EditorCamera( float fov, float aspectRatio, float nearPlane, float farPlane );

        void OnUpdate( primitive::Timestep deltaTime );
        void OnEvent( window::Event &e );

        float GetDistance() const
        {
            return m_Distance;
        }
        void SetDistance( float distance )
        {
            m_Distance = distance;
        }

        void SetViewportSize( float width, float height )
        {
            m_ViewportWidth = width;
            m_ViewportHeight = height;
            UpdateProjection();
        }

        const DirectX::XMFLOAT4X4 &GetViewMatrix() const
        {
            return m_ViewMatrix;
        }
        DirectX::XMFLOAT4X4 GetViewProjectionMatrix() const;

        DirectX::XMFLOAT3 GetUpDirection() const;
        DirectX::XMFLOAT3 GetRightDirection() const;
        DirectX::XMFLOAT3 GetForwardDirection() const;
        const DirectX::XMFLOAT3 &GetPosition() const
        {
            return m_Position;
        }

        float GetPitch() const
        {
            return m_Pitch;
        }
        float GetYaw() const
        {
            return m_Yaw;
        }

        DirectX::XMFLOAT4X4 GetTransform() const;

      private:
        void UpdateView();
        void UpdateProjection();

        bool OnMouseScroll( window::MouseScrolledEvent &e );

        void MousePan( const DirectX::XMFLOAT2 &delta );
        void MouseRotate( const DirectX::XMFLOAT2 &delta );
        void MouseZoom( float delta );

        DirectX::XMFLOAT3 CalculatePosition() const;

        DirectX::XMFLOAT2 CalculatePanSpeed() const;
        float CalculateRotationSpeed() const;
        float CalculateZoomSpeed() const;

      private:
        float m_FOV = 45.f;
        float m_AspectRatio = 1.778f;
        float m_NearPlane = 0.1f;
        float m_FarPlane = 2500.f;

        DirectX::XMFLOAT4X4 m_ViewMatrix{};
        DirectX::XMFLOAT3 m_Position{};
        DirectX::XMFLOAT3 m_FocalPoint{};

        DirectX::XMFLOAT2 m_InitialMousePosition{};

        float m_Distance = 10.f;
        float m_Pitch = 0.f;
        float m_Yaw = 0.f;

        float m_ViewportWidth = 1280.f;
        float m_ViewportHeight = 720.f;
    };
}
