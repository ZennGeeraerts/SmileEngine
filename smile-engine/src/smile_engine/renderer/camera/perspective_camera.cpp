#include "smpch.h"
#include "perspective_camera.h"

namespace smile::renderer
{
    using namespace DirectX;

    PerspectiveCamera::PerspectiveCamera( float fov, float aspect_ratio )
    {
        XMStoreFloat4x4( &projectionMatrix, XMMatrixPerspectiveFovLH( fov, aspect_ratio, 0.1f, 2500.f ) );

        XMStoreFloat4x4( &viewMatrix, DirectX::XMMatrixIdentity() );
        XMStoreFloat4x4( &viewProjectionMatrix, XMLoadFloat4x4( &viewMatrix ) * XMLoadFloat4x4( &projectionMatrix ) );
    }

    void PerspectiveCamera::setProjectionMatrix( float fov, float aspect_ratio )
    {
        XMStoreFloat4x4( &projectionMatrix, XMMatrixPerspectiveFovLH( fov, aspect_ratio, 0.1f, 2500.f ) );
    }

    void PerspectiveCamera::setPosition( const DirectX::XMFLOAT3 &new_position )
    {
        position = new_position;
        recalculateViewMatrix();
    }

    void PerspectiveCamera::setRotation( const DirectX::XMFLOAT3 &new_rotation )
    {
        rotation = new_rotation;
        recalculateViewMatrix();
    }

    void PerspectiveCamera::recalculateViewMatrix()
    {
        XMMATRIX transform = XMMatrixMultiply(
            XMMatrixRotationRollPitchYaw(
                XMConvertToRadians( rotation.x ), XMConvertToRadians( rotation.y ), XMConvertToRadians( rotation.z ) ),
            XMMatrixTranslation( position.x, position.y, position.z ) );
        XMStoreFloat4x4( &viewMatrix, XMMatrixInverse( nullptr, transform ) );

        XMStoreFloat4x4( &viewProjectionMatrix, XMLoadFloat4x4( &viewMatrix ) * XMLoadFloat4x4( &projectionMatrix ) );
    }
}