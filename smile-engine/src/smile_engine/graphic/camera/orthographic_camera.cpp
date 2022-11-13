#include "smpch.h"
#include "orthographic_camera.h"

namespace smile::graphic
{
    using namespace DirectX;

    OrthographicCamera::OrthographicCamera( float left, float right, float bottom, float top )
        : m_Position{ 0.f, 0.f, 0.f }
    {
        XMStoreFloat4x4(
            &m_ProjectionMatrix, XMMatrixOrthographicOffCenterLH( left, right, bottom, top, 0.1f, 2500.f ) );

        XMStoreFloat4x4( &m_ViewMatrix, DirectX::XMMatrixIdentity() );
        XMStoreFloat4x4(
            &m_ViewProjectionMatrix, XMLoadFloat4x4( &m_ViewMatrix ) * XMLoadFloat4x4( &m_ProjectionMatrix ) );
    }

    void OrthographicCamera::setPosition( const DirectX::XMFLOAT3 &position )
    {
        m_Position = position;
        recalculateViewMatrix();
    }

    void OrthographicCamera::setRotation( float rotation )
    {
        m_Rotation = rotation;
        recalculateViewMatrix();
    }

    void OrthographicCamera::recalculateViewMatrix()
    {
        XMMATRIX transform = XMMatrixMultiply( XMMatrixRotationZ( XMConvertToRadians( m_Rotation ) ),
            XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z ) );
        XMStoreFloat4x4( &m_ViewMatrix, XMMatrixInverse( nullptr, transform ) );

        XMStoreFloat4x4(
            &m_ViewProjectionMatrix, XMLoadFloat4x4( &m_ViewMatrix ) * XMLoadFloat4x4( &m_ProjectionMatrix ) );
    }
}
