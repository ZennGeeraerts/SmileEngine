/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/physics/physics_material.h"

namespace smile::physics::ecs
{
    struct CapsuleColliderComponent final
    {
        CapsuleColliderComponent() = default;
        CapsuleColliderComponent( const CapsuleColliderComponent & ) = default;

        float Radius = 0.5f;
        float Height = 1.0f;
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
    };
}