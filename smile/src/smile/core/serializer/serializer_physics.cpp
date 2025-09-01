/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "serializer.h"

#include "smile/core/yaml/math.h"

#include "smile/physics/ecs/rigidbody_component.h"
#include "smile/physics/ecs/box_collider_component.h"
#include "smile/physics/ecs/sphere_collider_component.h"
#include "smile/physics/ecs/capsule_collider_component.h"
#include "smile/physics/ecs/character_controller_component.h"

namespace smile::physics::serializer
{
    void Serialize( yaml::Emitter &output, world::Entity entity )
    {
        if ( entity.HasComponent< ecs::RigidbodyComponent >() )
        {
            output << YAML::Key << "RigidbodyComponent";
            output << YAML::BeginMap;

            auto &rigidbodyComponent = entity.GetComponent< ecs::RigidbodyComponent >();
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
        }

        if ( entity.HasComponent< ecs::BoxColliderComponent >() )
        {
            output << YAML::Key << "BoxColliderComponent";
            output << YAML::BeginMap;

            auto &boxColliderComponent = entity.GetComponent< ecs::BoxColliderComponent >();
            output << YAML::Key << "Size" << YAML::Value << boxColliderComponent.Box.Size;
            output << YAML::Key << "Offset" << YAML::Value << boxColliderComponent.Box.Center;
            output << YAML::Key << "bTrigger" << YAML::Value << boxColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << boxColliderComponent.ShowColliderBounds;

            /*auto& pPhysicsMaterial = boxColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< ecs::SphereColliderComponent >() )
        {
            output << YAML::Key << "SphereColliderComponent";
            output << YAML::BeginMap;

            auto &sphereColliderComponent = entity.GetComponent< ecs::SphereColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.Sphere.Radius;
            output << YAML::Key << "bTrigger" << YAML::Value << sphereColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << sphereColliderComponent.ShowColliderBounds;

            /*auto& pPhysicsMaterial = sphereColliderComponent.pPhysicsMaterial;
            output << YAML::Key << "StaticFriction" << YAML::Value << pPhysicsMaterial->StaticFriction;
            output << YAML::Key << "DynamicFriction" << YAML::Value << pPhysicsMaterial->DynamicFriction;
            output << YAML::Key << "Bounciness" << YAML::Value << pPhysicsMaterial->Bounciness;*/

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< ecs::CapsuleColliderComponent >() )
        {
            output << YAML::Key << "CapsuleColliderComponent";
            output << YAML::BeginMap;

            auto &capsuleColliderComponent = entity.GetComponent< ecs::CapsuleColliderComponent >();
            output << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.Capsule.Radius;
            output << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.Capsule.Height;
            output << YAML::Key << "bTrigger" << YAML::Value << capsuleColliderComponent.IsTrigger;
            output << YAML::Key << "bShowColliderBounds" << YAML::Value << capsuleColliderComponent.ShowColliderBounds;

            output << YAML::EndMap;
        }

        if ( entity.HasComponent< ecs::CharacterControllerComponent >() )
        {
            output << YAML::Key << "CharacterControllerComponent";
            output << YAML::BeginMap;

            const auto &characterControllerComponent = entity.GetComponent< ecs::CharacterControllerComponent >();
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
        }
    }

    void Deserialize( const yaml::Node &data, world::Entity entity )
    {
        auto rigidbodyComponent = data["RigidbodyComponent"];
        if ( rigidbodyComponent )
        {
            auto &rbc = entity.AddComponent< ecs::RigidbodyComponent >();

            rbc.BodyType = static_cast< RigidbodyType >( rigidbodyComponent["BodyType"].as< int >() );
            rbc.CollisionDetection =
                static_cast< CollisionDetectionType >( rigidbodyComponent["CollisionDetectionType"].as< int >() );

            auto physicsMaterial = rigidbodyComponent["PhysicsMaterial"];
            /*rbc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            rbc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            rbc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

            rbc.Mass = rigidbodyComponent["Mass"].as< float >();
            rbc.LinearDrag = rigidbodyComponent["LinearDrag"].as< float >();
            rbc.AngularDrag = rigidbodyComponent["AngularDrag"].as< float >();

            rbc.DisableGravity = rigidbodyComponent["bDisableGravity"].as< bool >();
            rbc.IsKinematic = rigidbodyComponent["bKinematic"].as< bool >();

            rbc.LockPositionX = rigidbodyComponent["bLockPositionX"].as< bool >();
            rbc.LockPositionY = rigidbodyComponent["bLockPositionY"].as< bool >();
            rbc.LockPositionZ = rigidbodyComponent["bLockPositionZ"].as< bool >();

            rbc.LockRotationX = rigidbodyComponent["bLockRotationX"].as< bool >();
            rbc.LockRotationY = rigidbodyComponent["bLockRotationY"].as< bool >();
            rbc.LockRotationZ = rigidbodyComponent["bLockRotationZ"].as< bool >();
        }

        auto boxColliderComponent = data["BoxColliderComponent"];
        if ( boxColliderComponent )
        {
            auto &bcc = entity.AddComponent< ecs::BoxColliderComponent >();

            bcc.Box.Size = boxColliderComponent["Size"].as< DirectX::XMFLOAT3 >();
            bcc.Box.Center = boxColliderComponent["Offset"].as< DirectX::XMFLOAT3 >();
            bcc.IsTrigger = boxColliderComponent["bTrigger"].as< bool >();
            bcc.ShowColliderBounds = boxColliderComponent["bShowColliderBounds"].as< bool >();

            /*auto physicsMaterial = rigidBodyComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }

        auto sphereColliderComponent = data["SphereColliderComponent"];
        if ( sphereColliderComponent )
        {
            auto &scc = entity.AddComponent< ecs::SphereColliderComponent >();

            scc.Sphere.Radius = sphereColliderComponent["Radius"].as< float >();
            scc.IsTrigger = sphereColliderComponent["bTrigger"].as< bool >();
            scc.ShowColliderBounds = sphereColliderComponent["bShowColliderBounds"].as< bool >();

            /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }

        auto capsuleColliderComponent = data["CapsuleColliderComponent"];
        if ( capsuleColliderComponent )
        {
            auto &ccc = entity.AddComponent< ecs::CapsuleColliderComponent >();

            ccc.Capsule.Radius = capsuleColliderComponent["Radius"].as< float >();
            ccc.Capsule.Height = capsuleColliderComponent["Height"].as< float >();
            ccc.IsTrigger = capsuleColliderComponent["bTrigger"].as< bool >();
            ccc.ShowColliderBounds = capsuleColliderComponent["bShowColliderBounds"].as< bool >();
        }

        auto characterControllerComponent = data["CharacterControllerComponent"];
        if ( characterControllerComponent )
        {
            auto &ccc = entity.AddComponent< ecs::CharacterControllerComponent >();

            ccc.Radius = characterControllerComponent["Radius"].as< float >();
            ccc.Height = characterControllerComponent["Height"].as< float >();
            ccc.ClimbingMode = static_cast< CharacterController::ClimbingModeType >(
                characterControllerComponent["ClimbingMode"].as< Uint32 >() );
            ccc.Name = characterControllerComponent["Name"].as< std::string >();
            ccc.CollisionGroups =
                static_cast< CollisionGroupFlag >( characterControllerComponent["CollisionGroups"].as< Uint32 >() );
            ccc.CollisionIgnoreGroups = static_cast< CollisionGroupFlag >(
                characterControllerComponent["CollisionIgnoreGroups"].as< Uint32 >() );
            ccc.CollisionFlags = static_cast< CharacterController::CollisionFlag >(
                characterControllerComponent["CollisionFlags"].as< Uint32 >() );

            /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }
    }
}