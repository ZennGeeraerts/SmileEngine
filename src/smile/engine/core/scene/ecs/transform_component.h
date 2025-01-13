/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include <DirectXMath.h>

namespace smile::scene::ecs
{
    struct TransformComponent final
    {
        enum class TransformChanged
        {
            None = BIT( 0 ),
            Translation = BIT( 1 ),
            Rotation = BIT( 2 ),
            Scale = BIT( 3 ),
        };

        TransformComponent() = default;
        TransformComponent( const TransformComponent & ) = default;
        TransformComponent( const DirectX::XMFLOAT3 &translation,
            const DirectX::XMFLOAT3 &rotation,
            const DirectX::XMFLOAT3 &scale )
            : Translation{ translation }, Rotation{ rotation }, Scale{ scale }
        {
        }

        DirectX::XMFLOAT4X4 GetTransform() const
        {
            DirectX::XMMATRIX transformMat =
                DirectX::XMMatrixScaling( Scale.x, Scale.y, Scale.z ) *
                DirectX::XMMatrixRotationRollPitchYaw( Rotation.x, Rotation.y, Rotation.z ) *
                DirectX::XMMatrixTranslation( Translation.x, Translation.y, Translation.z );

            DirectX::XMFLOAT4X4 transform{};
            DirectX::XMStoreFloat4x4( &transform, transformMat );
            return transform;
        }

        DirectX::XMFLOAT4X4 GetWorldTransform() const
        {
            DirectX::XMMATRIX worldTransformMat =
                DirectX::XMMatrixScaling( WorldScale.x, WorldScale.y, WorldScale.z ) *
                DirectX::XMMatrixRotationRollPitchYaw( WorldRotation.x, WorldRotation.y, WorldRotation.z ) *
                DirectX::XMMatrixTranslation( WorldTranslation.x, WorldTranslation.y, WorldTranslation.z );

            DirectX::XMFLOAT4X4 worldTransform{};
            DirectX::XMStoreFloat4x4( &worldTransform, worldTransformMat );
            return worldTransform;
        }

        DirectX::XMFLOAT3 GetForward()
        {
            DirectX::XMFLOAT3 forward{ 0, 0, 1 };
            RotateVector( forward );
            return forward;
        }

        DirectX::XMFLOAT3 GetRight()
        {
            DirectX::XMFLOAT3 right{ 1, 0, 0 };
            RotateVector( right );
            return right;
        }

        DirectX::XMFLOAT3 Translation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Rotation{ 0.f, 0.f, 0.f };
        DirectX::XMFLOAT3 Scale{ 1.f, 1.f, 1.f };

        DirectX::XMFLOAT3 WorldTranslation{ 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 WorldRotation{ 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 WorldScale{ 1.0f, 1.0f, 1.0f };

        Uint32 TransformChanged;

      private:
        void RotateVector( DirectX::XMFLOAT3 &v )
        {
            DirectX::XMVECTOR rotationVec =
                DirectX::XMQuaternionRotationRollPitchYaw( Rotation.x, Rotation.y, Rotation.z );
            auto rotationMat = DirectX::XMMatrixRotationQuaternion( rotationVec );

            DirectX::XMVECTOR vVec =
                DirectX::XMVector3TransformCoord( DirectX::XMVectorSet( v.x, v.y, v.z, 0 ), rotationMat );
            DirectX::XMStoreFloat3( &v, vVec );
        }
    };
}