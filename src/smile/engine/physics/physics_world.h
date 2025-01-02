/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "physics_material.h"
#include "rigidbody.h"
#include "character_controller.h"

#include "smile/engine/core/scene/ecs/transform_component.h"

#include <DirectXMath.h>

namespace smile::physics
{
    class PhysicsEngine;

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

    struct PhysicsWorldSettings final
    {
        DirectX::XMFLOAT3 Gravity = { 0, -9.81f, 0 };

        BroadPhaseType BroadPhaseAlgorithm = BroadPhaseType::AutomaticBoxPrune;
        FrictionType FrictionModel = FrictionType::Patch;
        DirectX::XMFLOAT3 WorldBoundsMin = { 0, 0, 0 };
        DirectX::XMFLOAT3 WorldBoundsMax = { 1, 1, 1 };
        Uint32 WorldBoundsSubdivisions = 2;
        Uint32 SolverIterations = 6;
        Uint32 SolverVelocityIterations = 1;
        Ref< PhysicsMaterial > pDefaultPhysicsMaterial = CreateRef< PhysicsMaterial >();
    };

    class PhysicsWorld final
    {
      private:
        struct Opaque;

      public:
        PhysicsWorld( const PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings );
        ~PhysicsWorld();

        Ref< Rigidbody > CreateRigidbody( RigidbodyType bodyType, const DirectX::XMFLOAT4X4 &initialTransform );
        void DestroyRigidbody( Ref< Rigidbody > pRigidbody );

        Ref< CharacterController > CreateCharacterController( float radius,
            float height,
            CharacterController::ClimbingModeType climbingMode,
            const DirectX::XMFLOAT3 &initialTranslation );
        void DestroyCharacterController( Ref< CharacterController > pCharacterController );

        void OnSimulate( primitive::Timestep fixedDeltaTime );
        void OnDebugRender();

        void *GetInternal() const;
        void *GetControllerManager() const;
        const PhysicsWorldSettings &GetSettings() const;

      private:
        compiled::PImpl< Opaque > m_pImplementation;
    };
}