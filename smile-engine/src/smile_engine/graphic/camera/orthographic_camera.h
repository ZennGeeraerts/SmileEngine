#pragma once

namespace smile::graphic
{
    class OrthographicCamera final
    {
      public:
        OrthographicCamera( float left, float right, float bottom, float top );

        virtual const DirectX::XMFLOAT3 &getPosition() const
        {
            return position;
        }
        virtual float getRotation() const
        {
            return rotation;
        }
        virtual const DirectX::XMFLOAT4X4 &getProjectionMatrix() const
        {
            return projectionMatrix;
        }
        virtual const DirectX::XMFLOAT4X4 &getViewMatrix() const
        {
            return viewMatrix;
        }
        virtual const DirectX::XMFLOAT4X4 &getViewProjectionMatrix() const
        {
            return viewProjectionMatrix;
        }

        virtual void setPosition( const DirectX::XMFLOAT3 &new_position );
        virtual void setRotation( float new_rotation );

      private:
        virtual void recalculateViewMatrix();

      private:
        DirectX::XMFLOAT4X4 projectionMatrix;
        DirectX::XMFLOAT4X4 viewMatrix;
        DirectX::XMFLOAT4X4 viewProjectionMatrix;

        DirectX::XMFLOAT3 position;
        float rotation = 0.0f;
    };
}
