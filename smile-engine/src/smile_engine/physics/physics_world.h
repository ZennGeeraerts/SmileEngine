/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "physics_material.h"
#include "smile_engine/common/compiled/compiled.h"

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

    class PhysicsWorld
    {
      public:
        PhysicsWorld( PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings );

        virtual void *GetInternal() const = 0;

      protected:
        PhysicsEngine *m_pPhysicsEngine;
        PhysicsWorldSettings m_Settings;
    };
}