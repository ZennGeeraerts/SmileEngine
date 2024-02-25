/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/scene/entity.h"

namespace physx
{
    class PxController;
}

namespace smile::physics
{
    class CharacterController final
    {
      public:
        CharacterController( scene::Entity entity );

        void UpdateTransform();

        void Translate( const DirectX::XMFLOAT3 &translation );
        void Move( const DirectX::XMFLOAT3 &displacement, float minDist = 0 );

        void SetCollisionGroups( const CollisionGroupFlag groups );
        void SetCollisionIgnoreGroups( const CollisionGroupFlag ignoreGroups );

        DirectX::XMFLOAT3 GetPosition() const;
        DirectX::XMFLOAT3 GetFootPosition() const;

      private:
        scene::Entity m_Entity;
        physx::PxController *m_pController;
    };
}