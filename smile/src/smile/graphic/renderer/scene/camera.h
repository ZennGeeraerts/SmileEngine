/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <DirectXMath.h>

namespace smile::graphic
{
    class Camera
    {
      public:
        Camera()
        {
            DirectX::XMStoreFloat4x4( &m_ProjectionMatrix, DirectX::XMMatrixIdentity() );
        }

        Camera( const DirectX::XMFLOAT4X4 &projectionMatrix ) : m_ProjectionMatrix{ projectionMatrix }
        {
        }

        Camera( const DirectX::XMMATRIX &projectionMatrix )
        {
            DirectX::XMStoreFloat4x4( &m_ProjectionMatrix, projectionMatrix );
        }

        virtual ~Camera() = default;

        const DirectX::XMFLOAT4X4 &GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

      protected:
        DirectX::XMFLOAT4X4 m_ProjectionMatrix;
    };
}