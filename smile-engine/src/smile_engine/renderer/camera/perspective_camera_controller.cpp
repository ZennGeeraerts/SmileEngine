#include "smpch.h"
#include "perspective_camera_controller.h"

#include "smile_engine/input/input.h"
#include "smile_engine/input/key_codes.h"

namespace smile::renderer
{
    PerspectiveCameraController::PerspectiveCameraController( float fov, float aspect_ratio )
        : fov{ fov }, aspectRatio{ aspect_ratio }, camera{ fov, aspect_ratio }
    {
    }

    void PerspectiveCameraController::onUpdate( Timestep delta_time )
    {
        if ( input::Input::isKeyPressed( input::key::Left ) )
            cameraRotation.y -= DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
        if ( input::Input::isKeyPressed( input::key::Right ) )
            cameraRotation.y += DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
        if ( input::Input::isKeyPressed( input::key::Up ) )
            cameraRotation.x -= DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );
        if ( input::Input::isKeyPressed( input::key::Down ) )
            cameraRotation.x += DirectX::XMConvertToRadians( cameraRotationSpeed * delta_time );

        const DirectX::XMFLOAT3 forward = { 0.f, 0.f, 1.f };
        const DirectX::XMFLOAT3 right = { 1.f, 0.f, 0.f };
        DirectX::XMFLOAT3 dir{};

        if ( input::Input::isKeyPressed( 'A' ) )
            dir.x -= 1;
        if ( input::Input::isKeyPressed( 'D' ) )
            dir.x += 1;
        if ( input::Input::isKeyPressed( 'S' ) )
            dir.z -= 1;
        if ( input::Input::isKeyPressed( 'W' ) )
            dir.z += 1;
        if ( input::Input::isKeyPressed( input::key::Space ) )
            dir.y += 1;
        if ( input::Input::isKeyPressed( input::key::CtrlLeft ) )
            dir.y -= 1;

        dir.x = forward.x * dir.z + right.x * dir.x;
        // dir.y = forward.y * dir.z + right.y * dir.x;
        dir.z = forward.z * dir.z + right.z * dir.x;

        auto dir_mat = DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &dir ) );
        DirectX::XMStoreFloat3( &dir, dir_mat );

        cameraPosition.x += dir.x * cameraMoveSpeed * delta_time;
        cameraPosition.y += dir.y * cameraMoveSpeed * delta_time;
        cameraPosition.z += dir.z * cameraMoveSpeed * delta_time;

        camera.setPosition( cameraPosition );
        camera.setRotation( cameraRotation );
    }

    void PerspectiveCameraController::onEvent( Event &e )
    {
        EventDispatcher dispatcher{ e };
        dispatcher.dispatch< MouseScrolledEvent >( SM_BIND_EVENT_FN( PerspectiveCameraController::onMouseScrolled ) );
        dispatcher.dispatch< WindowResizeEvent >(
            SM_BIND_EVENT_FN( PerspectiveCameraController::onWindowResizedEvent ) );
    }

    bool PerspectiveCameraController::onMouseScrolled( MouseScrolledEvent &e )
    {
        zoomLevel -= e.getOffsetY();
        zoomLevel = std::max( zoomLevel, 0.25f );
        return false;
    }

    bool PerspectiveCameraController::onWindowResizedEvent( WindowResizeEvent &e )
    {
        aspectRatio = e.getWidth() / static_cast< float >( e.getHeight() );
        camera.setProjectionMatrix( fov, aspectRatio );
        return false;
    }
}