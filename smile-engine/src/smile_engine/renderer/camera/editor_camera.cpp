#include "smpch.h"
#include "editor_camera.h"

#include "smile_engine/input/input.h"
#include "smile_engine/input/key_codes.h"
#include "smile_engine/input/mouse_codes.h"

#include "smile_engine/math/math.h"

namespace smile::renderer
{
    EditorCamera::EditorCamera( float fov, float aspect_ratio, float near_plane, float far_plane )
        : fov{ fov },
          aspectRatio{ aspect_ratio },
          nearPlane{ near_plane },
          farPlane{ far_plane },
          Camera( DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( fov ),
              aspect_ratio,
              near_plane,
              far_plane ) )
    {
    }

    void EditorCamera::onUpdate( Timestep delta_time )
    {
        if ( input::Input::isKeyPressed( input::key::Alt ) )
        {
            DirectX::XMFLOAT2 mouse_position{ input::Input::getMouseX(), input::Input::getMouseY() };
            auto mouse_position_vec = DirectX::XMLoadFloat2( &mouse_position );
            auto initial_mouse_pos_vec = DirectX::XMLoadFloat2( &initialMousePosition );

            DirectX::XMVECTOR delta_vec =
                DirectX::XMVectorScale( ( DirectX::XMVectorSubtract( mouse_position_vec, initial_mouse_pos_vec ) ), 0.003f );
            initialMousePosition = mouse_position;

            DirectX::XMFLOAT2 delta{};
            DirectX::XMStoreFloat2( &delta, delta_vec );

            if ( input::Input::isMouseButtonPressed( input::mouse::ButtonMiddle ) )
                mousePan( delta );
            else if ( input::Input::isMouseButtonPressed( input::mouse::ButtonLeft ) )
                mouseRotate( delta );
            else if ( input::Input::isMouseButtonPressed( input::mouse::ButtonRight ) )
                mouseZoom( delta.y );
        }

        updateView();
    }

    void EditorCamera::updateView()
    {
        position = calculatePosition();

        DirectX::XMMATRIX transform_mat =
            DirectX::XMMatrixMultiply( DirectX::XMMatrixRotationRollPitchYaw( -pitch, -yaw, 0.f ),
                DirectX::XMMatrixTranslation( position.x, position.y, position.z ) );
        XMStoreFloat4x4( &viewMatrix, DirectX::XMMatrixInverse( nullptr, transform_mat ) );
    }

    void EditorCamera::updateProjection()
    {
        aspectRatio = viewportWidth / viewportHeight;
        DirectX::XMMATRIX projection_matrix_mat = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians( fov ), aspectRatio, nearPlane, farPlane );
        DirectX::XMStoreFloat4x4( &projectionMatrix, projection_matrix_mat );
    }

    void EditorCamera::onEvent( Event &e )
    {
        EventDispatcher dispatcher{ e };
        dispatcher.dispatch< MouseScrolledEvent >( SM_BIND_EVENT_FN( EditorCamera::onMouseScroll ) );
    }

    DirectX::XMFLOAT4X4 EditorCamera::getViewProjectionMatrix() const
    {
        DirectX::XMMATRIX view_projection_matrix_mat =
            DirectX::XMLoadFloat4x4( &viewMatrix ) * DirectX::XMLoadFloat4x4( &projectionMatrix );
        DirectX::XMFLOAT4X4 view_projection_matrix{};
        DirectX::XMStoreFloat4x4( &view_projection_matrix, view_projection_matrix_mat );
        return view_projection_matrix;
    }

    bool EditorCamera::onMouseScroll( MouseScrolledEvent &e )
    {
        float delta = e.getOffsetX() * 0.1f;
        mouseZoom( delta );
        updateView();
        return false;
    }

    void EditorCamera::mousePan( const DirectX::XMFLOAT2 &delta )
    {
        DirectX::XMFLOAT2 pan_speed = calculatePanSpeed();
        DirectX::XMFLOAT3 right_direction = getRightDirection();
        DirectX::XMVECTOR right_direction_vec = DirectX::XMLoadFloat3( &right_direction );
        DirectX::XMFLOAT3 up_direction = getUpDirection();
        DirectX::XMVECTOR up_direction_vec = DirectX::XMLoadFloat3( &up_direction );
        DirectX::XMVECTOR focal_point_vec = DirectX::XMLoadFloat3( &focalPoint );

        focal_point_vec = DirectX::XMVectorAdd( focal_point_vec,
            DirectX::XMVectorScale(
                DirectX::XMVectorScale(
                    DirectX::XMVectorScale( DirectX::XMVectorNegate( right_direction_vec ), delta.x ), pan_speed.x ),
                distance ) );

        focal_point_vec = DirectX::XMVectorAdd( focal_point_vec,
            DirectX::XMVectorScale(
                DirectX::XMVectorScale( DirectX::XMVectorScale( up_direction_vec, delta.y ), pan_speed.y ), distance ) );

        DirectX::XMStoreFloat3( &focalPoint, focal_point_vec );
    }

    DirectX::XMFLOAT2 EditorCamera::calculatePanSpeed() const
    {
        float x = std::min( viewportWidth / 1000.0f, 2.4f ); // max = 2.4f
        float x_factor = 0.0366f * ( x * x ) - 0.1778f * x + 0.3021f;

        float y = std::min( viewportHeight / 1000.0f, 2.4f ); // max = 2.4f
        float y_factor = 0.0366f * ( y * y ) - 0.1778f * y + 0.3021f;

        return DirectX::XMFLOAT2{ x_factor, y_factor };
    }

    void EditorCamera::mouseRotate( const DirectX::XMFLOAT2 &delta )
    {
        const float yaw_sign = getUpDirection().y < 0 ? -1.0f : 1.0f;
        yaw += yaw_sign * delta.x * calculateRotationSpeed();
        pitch += delta.y * calculateRotationSpeed();
    }

    float EditorCamera::calculateRotationSpeed() const
    {
        return 0.8f;
    }

    void EditorCamera::mouseZoom( float delta )
    {
        distance -= delta * calculateZoomSpeed();
        if ( distance < 1.0f )
        {
            DirectX::XMVECTOR focal_point_vec = DirectX::XMLoadFloat3( &focalPoint );
            DirectX::XMFLOAT3 forward_direction = getForwardDirection();
            focal_point_vec = DirectX::XMVectorAdd( focal_point_vec, DirectX::XMLoadFloat3( &forward_direction ) );
            DirectX::XMStoreFloat3( &focalPoint, focal_point_vec );
            distance = 1.0f;
        }
    }

    float EditorCamera::calculateZoomSpeed() const
    {
        float new_distance = distance * 0.2f;
        new_distance = std::max( new_distance, 0.0f );
        float speed = new_distance * new_distance;
        speed = std::min( speed, 100.0f ); // max speed = 100
        return speed;
    }

    DirectX::XMFLOAT3 EditorCamera::calculatePosition() const
    {
        DirectX::XMFLOAT3 position{};
        DirectX::XMFLOAT3 forward_direction = getForwardDirection();
        DirectX::XMVECTOR position_vec = DirectX::XMVectorSubtract( DirectX::XMLoadFloat3( &focalPoint ),
            DirectX::XMVectorScale( DirectX::XMLoadFloat3( &forward_direction ), distance ) );

        DirectX::XMStoreFloat3( &position, position_vec );
        return position;
    }

    DirectX::XMFLOAT3 EditorCamera::getUpDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR up_vec{ 0.f, 1.0f, 0.f };
        DirectX::XMVECTOR rotation_vec =
            DirectX::XMVector3Rotate( up_vec, DirectX::XMQuaternionRotationRollPitchYaw( -pitch, -yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotation_vec );
        return rotation;
    }

    DirectX::XMFLOAT3 EditorCamera::getRightDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR right_direction{ 1.0f, 0.0f, 0.0f };
        DirectX::XMVECTOR rotation_vec = DirectX::XMVector3Rotate(
            right_direction, DirectX::XMQuaternionRotationRollPitchYaw( -pitch, -yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotation_vec );
        return rotation;
    }

    DirectX::XMFLOAT3 EditorCamera::getForwardDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR forward_direction{ 0.0f, 0.0f, 1.0f };
        DirectX::XMVECTOR rotation_vec = DirectX::XMVector3Rotate(
            forward_direction, DirectX::XMQuaternionRotationRollPitchYaw( -pitch, -yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotation_vec );
        return rotation;
    }
}
