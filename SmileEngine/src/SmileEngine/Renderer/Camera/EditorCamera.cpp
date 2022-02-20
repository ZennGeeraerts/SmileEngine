#include "smpch.h"
#include "EditorCamera.h"

#include "SmileEngine/Core/Input.h"
#include "SmileEngine/Core/KeyCodes.h"
#include "SmileEngine/Core/MouseCodes.h"

#include "SmileEngine/Math/Math.h"

namespace smile
{
    EditorCamera::EditorCamera( float fov, float aspectRatio, float nearPlane, float farPlane )
        : m_FOV{ fov },
          m_AspectRatio{ aspectRatio },
          m_NearPlane{ nearPlane },
          m_FarPlane{ farPlane },
          Camera( DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( fov ),
              aspectRatio,
              nearPlane,
              farPlane ) )
    {
    }

    void EditorCamera::OnUpdate( Timestep deltaTime )
    {
        if ( Input::IsKeyPressed( Key::Alt ) )
        {
            DirectX::XMFLOAT2 mousePosition{ Input::GetMouseX(), Input::GetMouseY() };
            auto mousePositionVec = DirectX::XMLoadFloat2( &mousePosition );
            auto initialMousePosVec = DirectX::XMLoadFloat2( &m_InitialMousePosition );

            DirectX::XMVECTOR deltaVec =
                DirectX::XMVectorScale( ( DirectX::XMVectorSubtract( mousePositionVec, initialMousePosVec ) ), 0.003f );
            m_InitialMousePosition = mousePosition;

            DirectX::XMFLOAT2 delta{};
            DirectX::XMStoreFloat2( &delta, deltaVec );

            if ( Input::IsMouseButtonPressed( Mouse::ButtonMiddle ) )
                MousePan( delta );
            else if ( Input::IsMouseButtonPressed( Mouse::ButtonLeft ) )
                MouseRotate( delta );
            else if ( Input::IsMouseButtonPressed( Mouse::ButtonRight ) )
                MouseZoom( delta.y );
        }

        UpdateView();
    }

    void EditorCamera::UpdateView()
    {
        m_Position = CalculatePosition();

        DirectX::XMMATRIX transformMat =
            DirectX::XMMatrixMultiply( DirectX::XMMatrixRotationRollPitchYaw( -m_Pitch, -m_Yaw, 0.f ),
                DirectX::XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z ) );
        XMStoreFloat4x4( &m_ViewMatrix, DirectX::XMMatrixInverse( nullptr, transformMat ) );
    }

    void EditorCamera::UpdateProjection()
    {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        DirectX::XMMATRIX projectionMatrixMat = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians( m_FOV ), m_AspectRatio, m_NearPlane, m_FarPlane );
        DirectX::XMStoreFloat4x4( &m_ProjectionMatrix, projectionMatrixMat );
    }

    void EditorCamera::OnEvent( Event &e )
    {
        EventDispatcher dispatcher{ e };
        dispatcher.Dispatch< MouseScrolledEvent >( SM_BIND_EVENT_FN( EditorCamera::OnMouseScroll ) );
    }

    DirectX::XMFLOAT4X4 EditorCamera::GetViewProjectionMatrix() const
    {
        DirectX::XMMATRIX viewProjectionMatrixMat =
            DirectX::XMLoadFloat4x4( &m_ViewMatrix ) * DirectX::XMLoadFloat4x4( &m_ProjectionMatrix );
        DirectX::XMFLOAT4X4 viewProjectionMatrix{};
        DirectX::XMStoreFloat4x4( &viewProjectionMatrix, viewProjectionMatrixMat );
        return viewProjectionMatrix;
    }

    bool EditorCamera::OnMouseScroll( MouseScrolledEvent &e )
    {
        float delta = e.GetOffsetX() * 0.1f;
        MouseZoom( delta );
        UpdateView();
        return false;
    }

    void EditorCamera::MousePan( const DirectX::XMFLOAT2 &delta )
    {
        DirectX::XMFLOAT2 panSpeed = CalculatePanSpeed();
        DirectX::XMFLOAT3 rightDirection = GetRightDirection();
        DirectX::XMVECTOR rightDirectionVec = DirectX::XMLoadFloat3( &rightDirection );
        DirectX::XMFLOAT3 upDirection = GetUpDirection();
        DirectX::XMVECTOR upDirectionVec = DirectX::XMLoadFloat3( &upDirection );
        DirectX::XMVECTOR focalPointVec = DirectX::XMLoadFloat3( &m_FocalPoint );

        focalPointVec = DirectX::XMVectorAdd( focalPointVec,
            DirectX::XMVectorScale(
                DirectX::XMVectorScale(
                    DirectX::XMVectorScale( DirectX::XMVectorNegate( rightDirectionVec ), delta.x ), panSpeed.x ),
                m_Distance ) );

        focalPointVec = DirectX::XMVectorAdd( focalPointVec,
            DirectX::XMVectorScale(
                DirectX::XMVectorScale( DirectX::XMVectorScale( upDirectionVec, delta.y ), panSpeed.y ), m_Distance ) );

        DirectX::XMStoreFloat3( &m_FocalPoint, focalPointVec );
    }

    DirectX::XMFLOAT2 EditorCamera::CalculatePanSpeed() const
    {
        float x = std::min( m_ViewportWidth / 1000.0f, 2.4f ); // max = 2.4f
        float xFactor = 0.0366f * ( x * x ) - 0.1778f * x + 0.3021f;

        float y = std::min( m_ViewportHeight / 1000.0f, 2.4f ); // max = 2.4f
        float yFactor = 0.0366f * ( y * y ) - 0.1778f * y + 0.3021f;

        return DirectX::XMFLOAT2{ xFactor, yFactor };
    }

    void EditorCamera::MouseRotate( const DirectX::XMFLOAT2 &delta )
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * CalculateRotationSpeed();
        m_Pitch += delta.y * CalculateRotationSpeed();
    }

    float EditorCamera::CalculateRotationSpeed() const
    {
        return 0.8f;
    }

    void EditorCamera::MouseZoom( float delta )
    {
        m_Distance -= delta * CalculateZoomSpeed();
        if ( m_Distance < 1.0f )
        {
            DirectX::XMVECTOR focalPointVec = DirectX::XMLoadFloat3( &m_FocalPoint );
            DirectX::XMFLOAT3 forwardDirection = GetForwardDirection();
            focalPointVec = DirectX::XMVectorAdd( focalPointVec, DirectX::XMLoadFloat3( &forwardDirection ) );
            DirectX::XMStoreFloat3( &m_FocalPoint, focalPointVec );
            m_Distance = 1.0f;
        }
    }

    float EditorCamera::CalculateZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = std::max( distance, 0.0f );
        float speed = distance * distance;
        speed = std::min( speed, 100.0f ); // max speed = 100
        return speed;
    }

    DirectX::XMFLOAT3 EditorCamera::CalculatePosition() const
    {
        DirectX::XMFLOAT3 position{};
        DirectX::XMFLOAT3 forwardDirection = GetForwardDirection();
        DirectX::XMVECTOR positionVec = DirectX::XMVectorSubtract( DirectX::XMLoadFloat3( &m_FocalPoint ),
            DirectX::XMVectorScale( DirectX::XMLoadFloat3( &forwardDirection ), m_Distance ) );

        DirectX::XMStoreFloat3( &position, positionVec );
        return position;
    }

    DirectX::XMFLOAT3 EditorCamera::GetUpDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR upVec{ 0.f, 1.0f, 0.f };
        DirectX::XMVECTOR rotationVec =
            DirectX::XMVector3Rotate( upVec, DirectX::XMQuaternionRotationRollPitchYaw( -m_Pitch, -m_Yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotationVec );
        return rotation;
    }

    DirectX::XMFLOAT3 EditorCamera::GetRightDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR rightDirection{ 1.0f, 0.0f, 0.0f };
        DirectX::XMVECTOR rotationVec = DirectX::XMVector3Rotate(
            rightDirection, DirectX::XMQuaternionRotationRollPitchYaw( -m_Pitch, -m_Yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotationVec );
        return rotation;
    }

    DirectX::XMFLOAT3 EditorCamera::GetForwardDirection() const
    {
        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR forwardDirection{ 0.0f, 0.0f, 1.0f };
        DirectX::XMVECTOR rotationVec = DirectX::XMVector3Rotate(
            forwardDirection, DirectX::XMQuaternionRotationRollPitchYaw( -m_Pitch, -m_Yaw, 0.f ) );
        DirectX::XMStoreFloat3( &rotation, rotationVec );
        return rotation;
    }
}
