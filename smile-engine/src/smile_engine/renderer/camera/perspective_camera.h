#pragma once

namespace smile
{
    class PerspectiveCamera final
    {
      public:
        PerspectiveCamera( float fov, float aspectRatio );

        void SetProjectionMatrix( float fov, float aspectRatio );

        const DirectX::XMFLOAT4X4 &GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }
        const DirectX::XMFLOAT4X4 &GetViewMatrix() const
        {
            return m_ViewMatrix;
        }
        const DirectX::XMFLOAT4X4 &GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

        void SetPosition( const DirectX::XMFLOAT3 &position );
        void SetRotation( const DirectX::XMFLOAT3 &rotation );

      private:
        virtual void RecalculateViewMatrix();

      private:
        DirectX::XMFLOAT4X4 m_ProjectionMatrix;
        DirectX::XMFLOAT4X4 m_ViewMatrix;
        DirectX::XMFLOAT4X4 m_ViewProjectionMatrix;

        DirectX::XMFLOAT3 m_Position = { 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 m_Rotation = { 0.f, 0.f, 0.f };
    };
}
