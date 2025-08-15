/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "capsule_collider_component.h"

#include "smile/core/world/component_serializer.h"
#include "smile/core/world/entity.h"

namespace smile::world
{
    template <>
    bool ComponentSerializer< physics::ecs::CapsuleColliderComponent >::TrySerialize( yaml::Emitter &output,
        Entity entity )
    {
        if ( !entity.HasComponent< physics::ecs::CapsuleColliderComponent >() )
        {
            return false;
        }

        output << YAML::Key << "CapsuleColliderComponent";
        output << YAML::BeginMap;

        auto &capsuleColliderComponent = entity.GetComponent< physics::ecs::CapsuleColliderComponent >();
        output << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.Capsule.Radius;
        output << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.Capsule.Height;
        output << YAML::Key << "bTrigger" << YAML::Value << capsuleColliderComponent.IsTrigger;
        output << YAML::Key << "bShowColliderBounds" << YAML::Value << capsuleColliderComponent.ShowColliderBounds;

        output << YAML::EndMap;

        return true;
    }

    template <>
    bool ComponentSerializer< physics::ecs::CapsuleColliderComponent >::TryDeserialize( const yaml::Node &data,
        Entity entity )
    {
        auto capsuleColliderData = data["CapsuleColliderComponent"];
        if ( !capsuleColliderData )
        {
            return false;
        }

        auto &capsuleColliderComponent = entity.AddComponent< physics::ecs::CapsuleColliderComponent >();

        capsuleColliderComponent.Capsule.Radius = capsuleColliderData["Radius"].as< float >();
        capsuleColliderComponent.Capsule.Height = capsuleColliderData["Height"].as< float >();
        capsuleColliderComponent.IsTrigger = capsuleColliderData["bTrigger"].as< bool >();
        capsuleColliderComponent.ShowColliderBounds = capsuleColliderData["bShowColliderBounds"].as< bool >();

        return true;
    }
}