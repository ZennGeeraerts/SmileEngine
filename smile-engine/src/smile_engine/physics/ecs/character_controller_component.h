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

        enum class CollisionFlag : Uint8
        {
            Sides = BIT( 0 ), // Character is colliding to the sides.
            Up = BIT( 1 ),    // Character has collision above.
            Down = BIT( 2 )   // Character has collision below.
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