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
            const PhysicsGeometry &geometry,
            Ref< PhysicsMaterial > pPhysicsMaterial );
        ~PhysicsShape();

        void SetTrigger( bool isTrigger );

      private:
        compiled::PImpl< Opaque > m_pImplementation;
    };
}