/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "physics_geometry.h"
#include "physics_material.h"

namespace smile::physics
{
    class Rigidbody;

    class PhysicsShape final
    {
      private:
        struct Opaque;

      public:
        PhysicsShape( const Rigidbody *pRigidbody,
            const PhysicsGeometry *pGeometry,
            Ref< PhysicsMaterial > pPhysicsMaterial );
        ~PhysicsShape();

        void SetTrigger( bool isTrigger );

      private:
        foundation::PImpl< Opaque > m_pImplementation;
    };
}