/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "physics_actor.h"
#include "character_controller.h"

namespace physx
{
    class PxPhysics;
    class PxScene;
    class PxControllerManager;
    class PxAllocatorCallback;
}

namespace smile::physics
{
    struct PhysicsEngineData final
    {
        float Accumulator = 0.0f;
        Uint32 SubstepCount = 0;
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
        float FixedTimestep = 0.01f;
        Uint32 MaxSubsteps = 8;
        DirectX::XMFLOAT3 Gravity = { 0, -9.81f, 0 };

        BroadPhaseType BroadPhaseAlgorithm = BroadPhaseType::AutomaticBoxPrune;
        FrictionType FrictionModel = FrictionType::Patch;
        DirectX::XMFLOAT3 WorldBoundsMin = { 0, 0, 0 };
        DirectX::XMFLOAT3 WorldBoundsMax = { 1, 1, 1 };
        Uint32 WorldBoundsSubdivisions = 2;
        Uint32 SolverIterations = 6;
        Uint32 SolverVelocityIterations = 1;
        Ref< PhysicsMaterial > pDefaultPhysicsMaterial;
    };

    class PhysicsEngine final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void CreateScene();
        static void DestroyScene();

        static Ref< PhysicsActor > CreateActor( scene::Entity entity );
        static void RemoveActor( scene::Entity entity );
        static bool IsPhysicsActor( scene::Entity entity );

        static Ref< CharacterController > CreateCharacterController( scene::Entity entity );
        static void RemoveCharacterController( scene::Entity entity );
        static bool IsCharacterController( scene::Entity entity );

        static Ref< PhysicsActor > GetActorOfEntity( scene::Entity entity );
        static Ref< CharacterController > GetCharacterControllerOfEntity( scene::Entity entity );

        static void Simulate( Timestep deltaTime );

        static physx::PxPhysics *GetPhysics();
        static physx::PxScene *GetScene();
        static physx::PxControllerManager *GetControllerManager();
        static physx::PxAllocatorCallback &GetAllocatorCallback();
        static const PhysicsSettings &GetPhysicsSettings()
        {
            return s_Settings;
        }
        static const Ref< PhysicsMaterial > &GetDefaultPhysicsMaterial()
        {
            return s_Settings.pDefaultPhysicsMaterial;
        }

      private:
        static bool Advance( Timestep deltaTime );
        static void SubstepStrategy( Timestep deltaTime );

      private:
        static std::unordered_map< UUID, Ref< PhysicsActor > > s_ActorMap;
        static std::unordered_map< UUID, Ref< CharacterController > > s_CharacterControllerMap;
        static PhysicsSettings s_Settings;
        static PhysicsEngineData s_PhysicsEngineData;
    };
}