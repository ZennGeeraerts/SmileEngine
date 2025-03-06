/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/singleton.h"
#include "physics_world.h"

namespace smile::physics
{
    struct PhysicsEngineData final
    {
        float FixedTimestep = 0.01f;
        Uint32 MaxSubsteps = 8;
        float Accumulator = 0.0f;
        Uint32 SubstepCount = 0;
    };

    class PhysicsEngine final : public foundation::Singleton< PhysicsEngine >
    {
      private:
        struct Opaque;

      public:
        PhysicsEngine();
        ~PhysicsEngine();

        Ref< PhysicsWorld > CreateWorld( const PhysicsWorldSettings &worldSettings );
        void DestroyWorld( Ref< PhysicsWorld > pPhysicsWorld );

        Ref< PhysicsMaterial > CreateMaterial( float staticFriction, float dynamicFriction, float restitution );
        void DestroyMaterial( Ref< PhysicsMaterial > pPhysicsMaterial );

        bool OnSimulate( primitive::Timestep deltaTime );

        void *GetPhysics() const;
        void *GetDefaultCpuDispatcher() const;
        const std::vector< Ref< PhysicsWorld > > &GetWorlds() const;

      private:
        bool Advance( primitive::Timestep deltaTime );
        void SubstepStrategy( primitive::Timestep deltaTime );

      private:
        foundation::PImpl< Opaque > m_pImplementation;
    };
}