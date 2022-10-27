#pragma once
#include <DirectXMath.h>

namespace smile::graphic
{
    class Camera
    {
      public:
        Camera()
        {
            DirectX::XMStoreFloat4x4( &projectionMatrix, DirectX::XMMatrixIdentity() );
        }

        Camera( const DirectX::XMFLOAT4X4 &projection_matrix ) : projectionMatrix{ projection_matrix }
        {
        }

        Camera( const DirectX::XMMATRIX &projection_matrix )
        {
            DirectX::XMStoreFloat4x4( &projectionMatrix, projection_matrix );
        }

        virtual ~Camera() = default;

        const DirectX::XMFLOAT4X4 &getProjectionMatrix() const
        {
            return projectionMatrix;
        };

      protected:
        DirectX::XMFLOAT4X4 projectionMatrix;
    };
}