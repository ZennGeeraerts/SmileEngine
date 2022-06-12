#include "smpch.h"
#include "orthographic_camera.h"

namespace smile::renderer
{
    using namespace DirectX;

    OrthographicCamera::OrthographicCamera( float left, float right, float bottom, float top )
        : position{ 0.f, 0.f, 0.f }
    {
        XMStoreFloat4x4(
            &projectionMatrix, XMMatrixOrthographicOffCenterLH( left, right, bottom, top, 0.1f, 2500.f ) );

        XMStoreFloat4x4( &viewMatrix, DirectX::XMMatrixIdentity() );
        XMStoreFloat4x4(
            &viewProjectionMatrix, XMLoadFloat4x4( &viewMatrix ) * XMLoadFloat4x4( &projectionMatrix ) );
    }

    void OrthographicCamera::setPosition( const DirectX::XMFLOAT3 &new_position )
    {
        position = new_position;
        recalculateViewMatrix();
    }

    void OrthographicCamera::setRotation( float new_rotation )
    {
        rotation = new_rotation;
        recalculateViewMatrix();
    }

    void OrthographicCamera::recalculateViewMatrix()
    {
        XMMATRIX transform = XMMatrixMultiply( XMMatrixRotationZ( XMConvertToRadians( rotation ) ),
            XMMatrixTranslation( position.x, position.y, position.z ) );
        XMStoreFloat4x4( &viewMatrix, XMMatrixInverse( nullptr, transform ) );

        XMStoreFloat4x4(
            &viewProjectionMatrix, XMLoadFloat4x4( &viewMatrix ) * XMLoadFloat4x4( &projectionMatrix ) );
    }
}
