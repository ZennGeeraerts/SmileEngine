/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "character_controller_component.h"

#include "smile/core/world/component_serializer.h"
#include "smile/core/world/entity.h"

namespace smile::world
{
    bool ComponentSerializer< physics::ecs::CharacterControllerComponent >::TrySerialize( yaml::Emitter &output,
        Entity entity )
    {
        if ( !entity.HasComponent< physics::ecs::CharacterControllerComponent >() )
        {
            return false;
        }

        output << YAML::Key << "CharacterControllerComponent";
        output << YAML::BeginMap;

        const auto &characterControllerComponent = entity.GetComponent< physics::ecs::CharacterControllerComponent >();
        output << YAML::Key << "Radius" << YAML::Value << characterControllerComponent.Radius;
        output << YAML::Key << "Height" << YAML::Value << characterControllerComponent.Height;
        output << YAML::Key << "ClimbingMode" << YAML::Value
               << static_cast< Uint32 >( characterControllerComponent.ClimbingMode );
        output << YAML::Key << "Name" << YAML::Value << characterControllerComponent.Name;
        output << YAML::Key << "CollisionGroups" << YAML::Value
               << static_cast< Uint32 >( characterControllerComponent.CollisionFlags );
        output << YAML::Key << "CollisionIgnoreGroups" << YAML::Value
               << static_cast< Uint32 >( characterControllerComponent.CollisionIgnoreGroups );
        output << YAML::Key << "CollisionFlags" << YAML::Value
               << static_cast< Uint32 >( characterControllerComponent.CollisionFlags );

        /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
        output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
        output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
        output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

        output << YAML::EndMap;

        return true;
    }

    bool ComponentSerializer< physics::ecs::CharacterControllerComponent >::TryDeserialize( const yaml::Node &data,
        Entity entity )
    {
        auto characterControllerData = data["CharacterControllerComponent"];
        if ( !characterControllerData )
        {
            return false;
        }

        auto &characterControllerComponent = entity.AddComponent< physics::ecs::CharacterControllerComponent >();

        characterControllerComponent.Radius = characterControllerData["Radius"].as< float >();
        characterControllerComponent.Height = characterControllerData["Height"].as< float >();

        characterControllerComponent.ClimbingMode = static_cast< physics::CharacterController::ClimbingModeType >(
            characterControllerData["ClimbingMode"].as< Uint32 >() );

        characterControllerComponent.Name = characterControllerData["Name"].as< std::string >();

        characterControllerComponent.CollisionGroups =
            static_cast< physics::CollisionGroupFlag >( characterControllerData["CollisionGroups"].as< Uint32 >() );

        characterControllerComponent.CollisionIgnoreGroups = static_cast< physics::CollisionGroupFlag >(
            characterControllerData["CollisionIgnoreGroups"].as< Uint32 >() );

        characterControllerComponent.CollisionFlags = static_cast< physics::CharacterController::CollisionFlag >(
            characterControllerData["CollisionFlags"].as< Uint32 >() );

        /*auto physicsMaterial = characterControllerData["PhysicsMaterial"];
        characterControllerComponent.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
        characterControllerComponent.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
        characterControllerComponent.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

        return true;
    }
}