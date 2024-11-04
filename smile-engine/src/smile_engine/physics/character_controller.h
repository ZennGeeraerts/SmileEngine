/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/core/scene/entity.h"
#include "collision_group.h"

namespace smile::physics
{
    class CharacterController
    {
      public:
        enum class CollisionFlag : Uint8
        {
            Sides = BIT( 0 ), // Character is colliding to the sides.
            Up = BIT( 1 ),    // Character has collision above.
            Down = BIT( 2 )   // Character has collision below.
        };

      public:
        CharacterController() = default;
        virtual ~CharacterController() = default;

        void UpdateTransform();

        virtual void Translate( const DirectX::XMFLOAT3 &translation ) = 0;
        virtual CollisionFlag Move( const DirectX::XMFLOAT3 &displacement, float minDist = 0 ) = 0;

        void SetCollisionGroups( const CollisionGroupFlag groups );
        void SetCollisionIgnoreGroups( const CollisionGroupFlag ignoreGroups );

        virtual DirectX::XMFLOAT3 GetPosition() const = 0;
        virtual DirectX::XMFLOAT3 GetFootPosition() const = 0;
    };
}