/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "perspective_camera_controller.h"

#include "engine/core/input/input.h"
#include "engine/core/input/key_codes.h"

namespace smile::graphic
{
    PerspectiveCameraController::PerspectiveCameraController( float fov, float aspectRatio )
        : m_FOV{ fov }, m_AspectRatio{ aspectRatio }, m_Camera{ fov, aspectRatio }
    {
    }

    void PerspectiveCameraController::OnUpdate( primitive::Timestep deltaTime )
    {
        if ( input::Input::IsKeyPressed( input::key::Left ) )
            m_CameraRotation.y -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
        if ( input::Input::IsKeyPressed( input::key::Right ) )
            m_CameraRotation.y += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
        if ( input::Input::IsKeyPressed( input::key::Up ) )
            m_CameraRotation.x -= DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );
        if ( input::Input::IsKeyPressed( input::key::Down ) )
            m_CameraRotation.x += DirectX::XMConvertToRadians( m_CameraRotationSpeed * deltaTime );

        const DirectX::XMFLOAT3 forward = { 0.f, 0.f, 1.f };
        const DirectX::XMFLOAT3 right = { 1.f, 0.f, 0.f };
        DirectX::XMFLOAT3 dir{};

        if ( input::Input::IsKeyPressed( 'A' ) )
            dir.x -= 1;
        if ( input::Input::IsKeyPressed( 'D' ) )
            dir.x += 1;
        if ( input::Input::IsKeyPressed( 'S' ) )
            dir.z -= 1;
        if ( input::Input::IsKeyPressed( 'W' ) )
            dir.z += 1;
        if ( input::Input::IsKeyPressed( input::key::Space ) )
            dir.y += 1;
        if ( input::Input::IsKeyPressed( input::key::CtrlLeft ) )
            dir.y -= 1;

        dir.x = forward.x * dir.z + right.x * dir.x;
        // dir.y = forward.y * dir.z + right.y * dir.x;
        dir.z = forward.z * dir.z + right.z * dir.x;

        auto dirMat = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &dir ) );
        DirectX::XMStoreFloat3( &dir, dirMat );

        m_CameraPosition.x += dir.x * m_CameraMoveSpeed * deltaTime;
        m_CameraPosition.y += dir.y * m_CameraMoveSpeed * deltaTime;
        m_CameraPosition.z += dir.z * m_CameraMoveSpeed * deltaTime;

        m_Camera.SetPosition( m_CameraPosition );
        m_Camera.SetRotation( m_CameraRotation );
    }

    void PerspectiveCameraController::OnEvent( window::Event & e )
    {
        window::EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< window::MouseScrolledEvent >( SM_BIND_EVENT_FN( PerspectiveCameraController::OnMouseScrolled ) );
        dispatcher.Dispatch< window::WindowResizeEvent >(
            SM_BIND_EVENT_FN( PerspectiveCameraController::OnWindowResizedEvent ) );
    }

    bool PerspectiveCameraController::OnMouseScrolled( window::MouseScrolledEvent &e )
    {
        m_ZoomLevel -= e.GetOffsetY();
        m_ZoomLevel = std::max( m_ZoomLevel, 0.25f );
        return false;
    }

    bool PerspectiveCameraController::OnWindowResizedEvent( window::WindowResizeEvent &e )
    {
        m_AspectRatio = e.GetWidth() / static_cast< float >( e.GetHeight() );
        m_Camera.SetProjectionMatrix( m_FOV, m_AspectRatio );
        return false;
    }
}