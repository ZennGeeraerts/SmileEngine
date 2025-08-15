/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "sphere_collider_component.h"

#include "smile/core/world/component_serializer.h"
#include "smile/core/world/entity.h"

namespace smile::world
{
    bool ComponentSerializer< physics::ecs::SphereColliderComponent >::TrySerialize( yaml::Emitter &output,
        Entity entity )
    {
        if ( !entity.HasComponent< physics::ecs::SphereColliderComponent >() )
        {
            return false;
        }

        output << YAML::Key << "SphereColliderComponent";
        output << YAML::BeginMap;

        auto &sphereColliderComponent = entity.GetComponent< physics::ecs::SphereColliderComponent >();
        output << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.Sphere.Radius;
        output << YAML::Key << "bTrigger" << YAML::Value << sphereColliderComponent.IsTrigger;
        output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphereColliderComponent.ShowColliderBounds;

        /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
        output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
        output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
        output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

        output << YAML::EndMap;

        return true;
    }

    template <>
    bool ComponentSerializer< physics::ecs::SphereColliderComponent >::TryDeserialize( const yaml::Node &data,
        Entity entity )
    {
        auto sphereColliderData = data["SphereColliderComponent"];
        if ( !sphereColliderData )
        {
            return false;
        }

        auto &sphereColliderComponent = entity.AddComponent< physics::ecs::SphereColliderComponent >();

        sphereColliderComponent.Sphere.Radius = sphereColliderData["Radius"].as< float >();
        sphereColliderComponent.IsTrigger = sphereColliderData["bTrigger"].as< bool >();
        sphereColliderComponent.ShowColliderBounds = sphereColliderData["bShowColliderBounds"].as< bool >();

        /*auto physicsMaterial = sphereColliderData["PhysicsMaterial"];
        sphereColliderComponent.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
        sphereColliderComponent.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
        sphereColliderComponent.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

        return true;
    }
}