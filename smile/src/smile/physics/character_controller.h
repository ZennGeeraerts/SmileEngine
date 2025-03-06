/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/pimpl.h"
#include "collision_group.h"

#include <DirectXMath.h>

namespace smile::physics
{
    class PhysicsWorld;

    class CharacterController final
    {
      private:
        struct Opaque;

      public:
        enum class ClimbingModeType : Uint8
        {
            Easy,
            Constrained,
            Last
        };

        enum class CollisionFlag : Uint8
        {
            Sides = BIT( 0 ), // Character is colliding to the sides.
            Up = BIT( 1 ),    // Character has collision above.
            Down = BIT( 2 )   // Character has collision below.
        };

      public:
        CharacterController( const PhysicsWorld *pPhysicsWorld,
            float radius,
            float height,
            ClimbingModeType climbingMode,
            const DirectX::XMFLOAT3 &initialTranslation );
        ~CharacterController();

        void Translate( const DirectX::XMFLOAT3 &translation );
        CollisionFlag Move( const DirectX::XMFLOAT3 &displacement, float minDist = 0 );

        void SetCollisionGroups( const CollisionGroupFlag groups, const CollisionGroupFlag ignoreGroups );

        void SetName( const std::string &name );

        DirectX::XMFLOAT3 GetPosition() const;
        DirectX::XMFLOAT3 GetFootPosition() const;

      private:
        foundation::PImpl< Opaque > m_pImplementation;
    };
}