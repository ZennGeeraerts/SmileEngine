/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "rigidbody_component.h"

#include "smile/core/world/component_serializer.h"
#include "smile/core/world/entity.h"

namespace smile::world
{
    template <>
    bool ComponentSerializer< physics::ecs::RigidbodyComponent >::TrySerialize( yaml::Emitter &output, Entity entity )
    {
        if ( !entity.HasComponent< physics::ecs::RigidbodyComponent >() )
        {
            return false;
        }

        output << YAML::Key << "RigidbodyComponent";
        output << YAML::BeginMap;

        auto &rigidbodyComponent = entity.GetComponent< physics::ecs::RigidbodyComponent >();
        output << YAML::Key << "BodyType" << YAML::Value << static_cast< Uint32 >( rigidbodyComponent.BodyType );
        output << YAML::Key << "CollisionDetectionType" << YAML::Value
               << static_cast< Uint32 >( rigidbodyComponent.CollisionDetection );

        output << YAML::Key << "PhysicsMaterial";
        output << YAML::BeginMap;

        /*auto& pPhysicsMaterial = rigidbodyComponent.pPhysicsMaterial;
        output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
        output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
        output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

        output << YAML::EndMap;

        output << YAML::Key << "Mass" << YAML::Value << rigidbodyComponent.Mass;
        output << YAML::Key << "LinearDrag" << YAML::Value << rigidbodyComponent.LinearDrag;
        output << YAML::Key << "AngularDrag" << YAML::Value << rigidbodyComponent.AngularDrag;
        output << YAML::Key << "bDisableGravity" << YAML::Value << rigidbodyComponent.DisableGravity;
        output << YAML::Key << "bKinematic" << YAML::Value << rigidbodyComponent.IsKinematic;

        output << YAML::Key << "bLockPositionX" << YAML::Value << rigidbodyComponent.LockPositionX;
        output << YAML::Key << "bLockPositionY" << YAML::Value << rigidbodyComponent.LockPositionY;
        output << YAML::Key << "bLockPositionZ" << YAML::Value << rigidbodyComponent.LockPositionZ;

        output << YAML::Key << "bLockRotationX" << YAML::Value << rigidbodyComponent.LockRotationX;
        output << YAML::Key << "bLockRotationY" << YAML::Value << rigidbodyComponent.LockRotationY;
        output << YAML::Key << "bLockRotationZ" << YAML::Value << rigidbodyComponent.LockRotationZ;

        output << YAML::EndMap;

        return true;
    }

    template <>
    bool ComponentSerializer< physics::ecs::RigidbodyComponent >::TryDeserialize( const yaml::Node &data,
        Entity entity )
    {
        auto rigidbodyData = data["RigidbodyComponent"];
        if ( !rigidbodyData )
        {
            return false;
        }

        auto &rigidbodyComponent = entity.AddComponent< physics::ecs::RigidbodyComponent >();

        rigidbodyComponent.BodyType = static_cast< physics::RigidbodyType >( rigidbodyData["BodyType"].as< int >() );
        rigidbodyComponent.CollisionDetection =
            static_cast< physics::CollisionDetectionType >( rigidbodyData["CollisionDetectionType"].as< int >() );

        /*auto physicsMaterial = rigidbodyData["PhysicsMaterial"];
        rigidbodyComponent.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
        rigidbodyComponent.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
        rigidbodyComponent.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

        rigidbodyComponent.Mass = rigidbodyData["Mass"].as< float >();
        rigidbodyComponent.LinearDrag = rigidbodyData["LinearDrag"].as< float >();
        rigidbodyComponent.AngularDrag = rigidbodyData["AngularDrag"].as< float >();

        rigidbodyComponent.DisableGravity = rigidbodyData["bDisableGravity"].as< bool >();
        rigidbodyComponent.IsKinematic = rigidbodyData["bKinematic"].as< bool >();

        rigidbodyComponent.LockPositionX = rigidbodyData["bLockPositionX"].as< bool >();
        rigidbodyComponent.LockPositionY = rigidbodyData["bLockPositionY"].as< bool >();
        rigidbodyComponent.LockPositionZ = rigidbodyData["bLockPositionZ"].as< bool >();

        rigidbodyComponent.LockRotationX = rigidbodyData["bLockRotationX"].as< bool >();
        rigidbodyComponent.LockRotationY = rigidbodyData["bLockRotationY"].as< bool >();
        rigidbodyComponent.LockRotationZ = rigidbodyData["bLockRotationZ"].as< bool >();

        return true;
    }
}