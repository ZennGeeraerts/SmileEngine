/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "box_collider_component.h"

#include "smile/core/world/component_serializer.h"
#include "smile/core/world/entity.h"
#include "smile/core/yaml/math.h"

namespace smile::world
{
    template <>
    bool ComponentSerializer< physics::ecs::BoxColliderComponent >::TrySerialize( yaml::Emitter &output, Entity entity )
    {
        if ( !entity.HasComponent< physics::ecs::BoxColliderComponent >() )
        {
            return false;
        }

        output << YAML::Key << "BoxColliderComponent";
        output << YAML::BeginMap;

        auto &boxColliderComponent = entity.GetComponent< physics::ecs::BoxColliderComponent >();
        output << YAML::Key << "Size" << YAML::Value << boxColliderComponent.Box.Size;
        output << YAML::Key << "Offset" << YAML::Value << boxColliderComponent.Box.Center;
        output << YAML::Key << "bTrigger" << YAML::Value << boxColliderComponent.IsTrigger;
        output << YAML::Key << "bShowColliderBounds" << YAML::Value << boxColliderComponent.ShowColliderBounds;

        /*auto& pPhysicsMaterial = boxColliderComponent.pPhysicsMaterial;
        output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
        output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
        output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

        output << YAML::EndMap;

        return true;
    }

    template <>
    bool ComponentSerializer< physics::ecs::BoxColliderComponent >::TryDeserialize( const yaml::Node &data,
        Entity entity )
    {
        auto boxColliderData = data["BoxColliderComponent"];
        if ( !boxColliderData )
        {
            return false;
        }

        auto &boxColliderComponent = entity.AddComponent< physics::ecs::BoxColliderComponent >();

        boxColliderComponent.Box.Size = boxColliderData["Size"].as< DirectX::XMFLOAT3 >();
        boxColliderComponent.Box.Center = boxColliderData["Offset"].as< DirectX::XMFLOAT3 >();
        boxColliderComponent.IsTrigger = boxColliderData["bTrigger"].as< bool >();
        boxColliderComponent.ShowColliderBounds = boxColliderData["bShowColliderBounds"].as< bool >();

        /*auto physicsMaterial = boxColliderData["PhysicsMaterial"];
        boxColliderComponent.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
        boxColliderComponent.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
        boxColliderComponent.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

        return true;
    }
}