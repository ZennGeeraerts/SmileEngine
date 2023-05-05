/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    class OrthographicCamera final
    {
      public:
        OrthographicCamera( float left, float right, float bottom, float top );

        virtual const DirectX::XMFLOAT3 &GetPosition() const
        {
            return m_Position;
        }
        virtual float GetRotation() const
        {
            return m_Rotation;
        }
        virtual const DirectX::XMFLOAT4X4 &GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }
        virtual const DirectX::XMFLOAT4X4 &GetViewMatrix() const
        {
            return m_ViewMatrix;
        }
        virtual const DirectX::XMFLOAT4X4 &GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

        virtual void setPosition( const DirectX::XMFLOAT3 &position );
        virtual void setRotation( float rotation );

      private:
        virtual void recalculateViewMatrix();

      private:
        DirectX::XMFLOAT4X4 m_ProjectionMatrix;
        DirectX::XMFLOAT4X4 m_ViewMatrix;
        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        DirectX::XMFLOAT3 m_Position;
        float m_Rotation = 0.0f;
    };
}
