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
        auto rigidbodyData = data["RigidbodyComponent"];
        if ( rigidbodyData )
        {
            auto &rigidbodyComponent = entity.AddComponent< ecs::RigidbodyComponent >();

            rigidbodyComponent.BodyType = static_cast< RigidbodyType >( rigidbodyData["BodyType"].as< int >() );
            rigidbodyComponent.CollisionDetection =
                static_cast< CollisionDetectionType >( rigidbodyData["CollisionDetectionType"].as< int >() );

            auto physicsMaterial = rigidbodyData["PhysicsMaterial"];
            /*rbc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            rbc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            rbc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/

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
        }

        auto boxColliderData = data["BoxColliderComponent"];
        if ( boxColliderData )
        {
            auto &boxColliderComponent = entity.AddComponent< ecs::BoxColliderComponent >();

            boxColliderComponent.Box.Size = boxColliderData["Size"].as< DirectX::XMFLOAT3 >();
            boxColliderComponent.Box.Center = boxColliderData["Offset"].as< DirectX::XMFLOAT3 >();
            boxColliderComponent.IsTrigger = boxColliderData["bTrigger"].as< bool >();
            boxColliderComponent.ShowColliderBounds = boxColliderData["bShowColliderBounds"].as< bool >();

            /*auto physicsMaterial = rigidBodyComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }

        auto sphereColliderData = data["SphereColliderComponent"];
        if ( sphereColliderData )
        {
            auto &sphereColliderComponent = entity.AddComponent< ecs::SphereColliderComponent >();

            sphereColliderComponent.Sphere.Radius = sphereColliderData["Radius"].as< float >();
            sphereColliderComponent.IsTrigger = sphereColliderData["bTrigger"].as< bool >();
            sphereColliderComponent.ShowColliderBounds = sphereColliderData["bShowColliderBounds"].as< bool >();

            /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }

        auto capsuleColliderData = data["CapsuleColliderComponent"];
        if ( capsuleColliderData )
        {
            auto &capsuleColliderComponent = entity.AddComponent< ecs::CapsuleColliderComponent >();

            capsuleColliderComponent.Capsule.Radius = capsuleColliderData["Radius"].as< float >();
            capsuleColliderComponent.Capsule.Height = capsuleColliderData["Height"].as< float >();
            capsuleColliderComponent.IsTrigger = capsuleColliderData["bTrigger"].as< bool >();
            capsuleColliderComponent.ShowColliderBounds = capsuleColliderData["bShowColliderBounds"].as< bool >();
        }

        auto characterControllerData = data["CharacterControllerComponent"];
        if ( characterControllerData )
        {
            auto &characterControllerComponent = entity.AddComponent< ecs::CharacterControllerComponent >();

            characterControllerComponent.Radius = characterControllerData["Radius"].as< float >();
            characterControllerComponent.Height = characterControllerData["Height"].as< float >();
            characterControllerComponent.ClimbingMode = static_cast< CharacterController::ClimbingModeType >(
                characterControllerData["ClimbingMode"].as< Uint32 >() );
            characterControllerComponent.Name = characterControllerData["Name"].as< std::string >();
            characterControllerComponent.CollisionGroups =
                static_cast< CollisionGroupFlag >( characterControllerData["CollisionGroups"].as< Uint32 >() );
            characterControllerComponent.CollisionIgnoreGroups =
                static_cast< CollisionGroupFlag >( characterControllerData["CollisionIgnoreGroups"].as< Uint32 >() );
            characterControllerComponent.CollisionFlags = static_cast< CharacterController::CollisionFlag >(
                characterControllerData["CollisionFlags"].as< Uint32 >() );

            /*auto physicsMaterial = sphereColliderComponent["PhysicsMaterial"];
            bcc.pPhysicsMaterial->StaticFriction = physicsMaterial["StaticFriction"].as<float>();
            bcc.pPhysicsMaterial->DynamicFriction = physicsMaterial["DynamicFriction"].as<float>();
            bcc.pPhysicsMaterial->Bounciness = physicsMaterial["Bounciness"].as<float>();*/
        }
    }
}