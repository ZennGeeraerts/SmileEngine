#pragma once

#include "physics_actor.h"

namespace physx
{
    class PxPhysics;
    class PxAllocatorCallback;
}

namespace smile
{
    struct PhysicsEngineData final
    {
        float m_Accumulator = 0.0f;
        Uint32 m_SubstepCount = 0;
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
        float m_FixedTimestep = 0.01f;
        Uint32 m_MaxSubsteps = 8;
        DirectX::XMFLOAT3 m_Gravity = { 0, -9.81f, 0 };

        BroadPhaseType m_BroadPhaseAlgorithm = BroadPhaseType::AutomaticBoxPrune;
        FrictionType m_FrictionModel = FrictionType::Patch;
        DirectX::XMFLOAT3 m_WorldBoundsMin = { 0, 0, 0 };
        DirectX::XMFLOAT3 m_WorldBoundsMax = { 1, 1, 1 };
        Uint32 m_WorldBoundsSubdivisions = 2;
        Uint32 m_SolverIterations = 6;
        Uint32 m_SolverVelocityIterations = 1;
        Ref< PhysicsMaterial > m_pDefaultPhysicsMaterial;
    };

    class PhysicsEngine final
    {
     public:
        static void Initialize();
        static void ShutDown();

        static void CreateScene();
        static void DestroyScene();

        static Ref< PhysicsActor > CreateActor( Entity entity );
        static Ref< PhysicsActor > GetActorOfEntity( Entity entity );

        static void Simulate( Timestep deltaTime );

        static physx::PxPhysics *GetPhysics();
        static physx::PxAllocatorCallback &GetAllocatorCallback();
        static const PhysicsSettings &GetPhysicsSettings()
        {
            return s_Settings;
        }
        static const Ref< PhysicsMaterial > &GetDefaultPhysicsMaterial()
        {
            return s_Settings.m_pDefaultPhysicsMaterial;
        }

     private:
        static bool Advance( Timestep deltaTime );
        static void SubstepStrategy( Timestep deltaTime );

     private:
        static std::unordered_map< UUID, Ref< PhysicsActor > > s_ActorMap;
        static PhysicsSettings s_Settings;
        static PhysicsEngineData s_PhysicsEngineData;
    };
}