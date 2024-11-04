/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physx_character_controller_manager.h"
#include "physx_physics_world.h"
#include "physx_character_controller.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    static physx::PxCapsuleClimbingMode::Enum ClimbingModeToPhysXType(
        ecs::CharacterControllerComponent::ClimbingModeType climbingMode )
    {
        switch ( climbingMode )
        {
            case ecs::CharacterControllerComponent::ClimbingModeType::Easy:
                return physx::PxCapsuleClimbingMode::Enum::eEASY;
            case ecs::CharacterControllerComponent::ClimbingModeType::Constrained:
                return physx::PxCapsuleClimbingMode::Enum::eCONSTRAINED;
            case ecs::CharacterControllerComponent::ClimbingModeType::Last:
                return physx::PxCapsuleClimbingMode::Enum::eLAST;
        }
    }

    PhysXCharacterControllerManager::PhysXCharacterControllerManager( PhysXPhysicsWorld *pPhysicsWorld )
        : CharacterControllerManager{ pPhysicsWorld }
    {
        auto pPhysXScene = static_cast< physx::PxScene * >( m_pPhysicsWorld->GetInternal() );
        m_pControllerManager = PxCreateControllerManager( *pPhysXScene );
    }

    Ref< CharacterController > PhysXCharacterControllerManager::CreateController(
        const ecs::CharacterControllerComponent &characterControllerComponent )
    {
        physx::PxCapsuleControllerDesc desc;
        desc.setToDefault();
        desc.radius = characterControllerComponent.Radius;
        desc.height = characterControllerComponent.Height;
        desc.climbingMode = ClimbingModeToPhysXType( characterControllerComponent.ClimbingMode );
        desc.upDirection = physx::PxVec3{ 0, 1, 0 };
        desc.contactOffset = 0.1f;

        auto pController = m_pControllerManager->createController( desc );
        auto pPhysXCharacterController = CreateRef< PhysXCharacterController >( pController );
        return pPhysXCharacterController;
    }
}