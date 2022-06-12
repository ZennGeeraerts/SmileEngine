#pragma once

#include "physics_actor.h"

namespace physx
{
    class PxPhysics;
    class PxAllocatorCallback;
}

namespace smile::physics
{
    struct PhysicsEngineData final
    {
        float accumulator = 0.0f;
        Uint32 substepCount = 0;
    };

    enum class FrictionType
    {
        Patch,
        OneDirectional,
        TwoDirectional
    };

    enum class BroadPhaseType
    {
        SweepAndPrune,
        MultiBoxPrune,
        AutomaticBoxPrune
    };

    struct PhysicsSettings final
    {
        float fixedTimestep = 0.01f;
        Uint32 maxSubsteps = 8;
        DirectX::XMFLOAT3 gravity = { 0, -9.81f, 0 };

        BroadPhaseType broadPhaseAlgorithm = BroadPhaseType::AutomaticBoxPrune;
        FrictionType frictionModel = FrictionType::Patch;
        DirectX::XMFLOAT3 worldBoundsMin = { 0, 0, 0 };
        DirectX::XMFLOAT3 worldBoundsMax = { 1, 1, 1 };
        Uint32 worldBoundsSubdivisions = 2;
        Uint32 solverIterations = 6;
        Uint32 solverVelocityIterations = 1;
        Ref< PhysicsMaterial > defaultPhysicsMaterial;
    };

    class PhysicsEngine final
    {
      public:
        static void initialize();
        static void shutDown();

        static void createScene();
        static void destroyScene();

        static Ref< PhysicsActor > createActor( scene::Entity entity );
        static Ref< PhysicsActor > getActorOfEntity( scene::Entity entity );

        static void simulate( Timestep delta_time );

        static physx::PxPhysics *getPhysics();
        static physx::PxAllocatorCallback &getAllocatorCallback();
        static const PhysicsSettings &getPhysicsSettings()
        {
            return settings;
        }
        static const Ref< PhysicsMaterial > &getDefaultPhysicsMaterial()
        {
            return settings.defaultPhysicsMaterial;
        }

      private:
        static bool advance( Timestep delta_time );
        static void substepStrategy( Timestep delta_time );

      private:
        static std::unordered_map< UUID, Ref< PhysicsActor > > actorMap;
        static PhysicsSettings settings;
        static PhysicsEngineData physicsEngineData;
    };
}