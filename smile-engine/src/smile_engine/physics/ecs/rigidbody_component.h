/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/physics/physics_material.h"

namespace smile::physics::ecs
{
    struct RigidbodyComponent final
    {
        enum class BodyType
        {
            Static = 0,
            Dynamic = 1
        };
        enum class CollisionDetectionType
        {
            Discrete = 0,
            Continuous = 1
        };

        RigidbodyComponent() = default;
        RigidbodyComponent( const RigidbodyComponent & ) = default;

        // Data
        BodyType Type = BodyType::Static;
        CollisionDetectionType CollisionDetection = CollisionDetectionType::Discrete;
        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;

        float Mass = 1.0f;
        float LinearDrag = 0.0f;
        float AngularDrag = 0.05f;

        bool DisableGravity = false;
        bool IsKinematic = false;

        bool LockPositionX = false;
        bool LockPositionY = false;
        bool LockPositionZ = false;
        bool LockRotationX = false;
        bool LockRotationY = false;
        bool LockRotationZ = false;
    };
}