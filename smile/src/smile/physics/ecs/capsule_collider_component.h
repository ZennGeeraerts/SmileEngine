/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/geometric/capsule.h"
#include "smile/physics/physics_material.h"

namespace smile::physics::ecs
{
    struct CapsuleColliderComponent final
    {
        CapsuleColliderComponent() = default;
        CapsuleColliderComponent( const CapsuleColliderComponent & ) = default;

        geometric::Capsule Capsule{ DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }, 0.5f, 1.0f };
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
    };
}