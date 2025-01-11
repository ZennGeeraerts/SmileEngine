/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/physics/rigidbody.h"

namespace smile::physics::ecs
{
    struct RigidbodyComponent final
    {
        RigidbodyComponent() = default;
        RigidbodyComponent( const RigidbodyComponent & ) = default;

        // Data
        RigidbodyType BodyType = RigidbodyType::Static;
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