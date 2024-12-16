/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/physics/character_controller.h"
#include "smile_engine/physics/collision_group.h"

namespace smile::physics::ecs
{
    struct CharacterControllerComponent final
    {
        CharacterControllerComponent() = default;
        CharacterControllerComponent( const CharacterControllerComponent & ) = default;

        float Radius = 2;
        float Height = 5;
        CharacterController::ClimbingModeType ClimbingMode = CharacterController::ClimbingModeType::Easy;
        std::string Name = "Character";

        physics::CollisionGroupFlag CollisionGroups = physics::CollisionGroupFlag::Group0;
        physics::CollisionGroupFlag CollisionIgnoreGroups{};

        CharacterController::CollisionFlag CollisionFlags{};

        Ref< PhysicsMaterial > pPhysicsMaterial = nullptr;
    };
}