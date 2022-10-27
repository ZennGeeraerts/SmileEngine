#pragma once

namespace smile::graphic
{
    class PerspectiveCamera final
    {
      public:
        PerspectiveCamera( float fov, float aspect_ratio );

        void setProjectionMatrix( float fov, float aspect_ratio );

        const DirectX::XMFLOAT4X4 &getProjectionMatrix() const
        {
            return projectionMatrix;
        }
        const DirectX::XMFLOAT4X4 &getViewMatrix() const
        {
            return viewMatrix;
        }
        const DirectX::XMFLOAT4X4 &getViewProjectionMatrix() const
        {
            return viewProjectionMatrix;
        }

        void setPosition( const DirectX::XMFLOAT3 &new_position );
        void setRotation( const DirectX::XMFLOAT3 &new_rotation );

      private:
        virtual void recalculateViewMatrix();

      private:
        DirectX::XMFLOAT4X4 projectionMatrix;
        DirectX::XMFLOAT4X4 viewMatrix;
        DirectX::XMFLOAT4X4 viewProjectionMatrix;

        DirectX::XMFLOAT3 position = { 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 rotation = { 0.f, 0.f, 0.f };
    };
}
