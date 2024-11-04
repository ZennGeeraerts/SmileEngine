/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/physics/collision_group.h"

namespace smile::physics::ecs
{
    struct CharacterControllerComponent final
    {
        enum class ClimbingModeType : Uint8
        {
            Easy,
            Constrained,
            Last
        };

        CharacterControllerComponent() = default;
        CharacterControllerComponent( const CharacterControllerComponent & ) = default;

        float Radius = 2;
        float Height = 5;
        ClimbingModeType ClimbingMode = ClimbingModeType::Easy;
        std::string Name = "Character";

        physics::CollisionGroupFlag CollisionGroups = physics::CollisionGroupFlag::Group0;
        physics::CollisionGroupFlag CollisionIgnoreGroups{};

        CollisionFlag CollisionFlags{};

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
    };
}