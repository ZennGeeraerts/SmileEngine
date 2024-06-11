/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "perspective_camera.h"
#include "smile_engine/common/primitive/timestep.h"

#include "smile_engine/core/window/events/application_event.h"
#include "smile_engine/core/window/events/mouse_event.h"

#include "smile_engine/core/scene/components.h"

namespace smile::graphic
{
    class PerspectiveCameraController final
    {
     public:
        PerspectiveCameraController( float fov, float aspectRatio );

        void OnUpdate( primitive::Timestep deltaTime );
        void OnEvent( window::Event &e );

     private:
        bool OnMouseScrolled( window::MouseScrolledEvent &e );
        bool OnWindowResizedEvent( window::WindowResizeEvent &e );

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
