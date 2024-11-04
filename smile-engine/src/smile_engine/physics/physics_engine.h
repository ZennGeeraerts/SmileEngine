/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "physics_world.h"
#include "physics_actor.h"
#include "character_controller.h"

#include "smile_engine/common/compiled/singleton.h"

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

    class PhysicsEngine : public compiled::Singleton< PhysicsEngine >
    {
      public:
        PhysicsEngine() = default;
        virtual ~PhysicsEngine() = default;

        virtual void CreateWorld() = 0;
        virtual void DestroyWorld() = 0;

        virtual void *GetPhysics() const = 0;

        Ref< PhysicsActor > CreateActor( scene::Entity entity );
        void RemoveActor( scene::Entity entity );
        bool IsPhysicsActor( scene::Entity entity );

        Ref< CharacterController > CreateCharacterController( scene::Entity entity );
        void RemoveCharacterController( scene::Entity entity );
        bool IsCharacterController( scene::Entity entity );

        Ref< PhysicsActor > GetActorOfEntity( scene::Entity entity );
        Ref< CharacterController > GetCharacterControllerOfEntity( scene::Entity entity );

        void Simulate( primitive::Timestep deltaTime );

        physx::PxPhysics *GetPhysics();
        physx::PxScene *GetScene();
        physx::PxControllerManager *GetControllerManager();
        physx::PxAllocatorCallback &GetAllocatorCallback();
        const PhysicsSettings &GetPhysicsSettings()
        {
            return s_Settings;
        }
        const Ref< PhysicsMaterial > &GetDefaultPhysicsMaterial()
        {
            return s_Settings.pDefaultPhysicsMaterial;
        }

      private:
        bool Advance( primitive::Timestep deltaTime );
        void SubstepStrategy( primitive::Timestep deltaTime );

      protected:
        std::vector< Scope< PhysicsWorld > > m_pWorlds{};

      private:
        PhysicsSettings s_Settings;
        PhysicsEngineData s_PhysicsEngineData;
    };
}