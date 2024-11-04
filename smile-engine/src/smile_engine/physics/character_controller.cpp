/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "character_controller.h"

#include "physics_engine.h"
#include "physics_utils.h"

#include "smile_engine/core/scene/components.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    CharacterController::CharacterController( scene::Entity entity ) : m_Entity{ entity }
    {
        auto pControllerManager = PhysicsEngine::GetControllerManager();

        auto &characterControllerComponent = entity.GetComponent< ecs::CharacterControllerComponent >();
        if ( !characterControllerComponent.pPhysicsMaterial )
            characterControllerComponent.pPhysicsMaterial = PhysicsEngine::GetDefaultPhysicsMaterial();

        physx::PxCapsuleControllerDesc desc;
        desc.setToDefault();
        desc.radius = characterControllerComponent.Radius;
        desc.height = characterControllerComponent.Height;
        desc.climbingMode = ClimbingModeToPhysXType( characterControllerComponent.ClimbingMode );
        desc.upDirection = physx::PxVec3{ 0, 1, 0 };
        desc.contactOffset = 0.1f;

        const auto translation = entity.GetComponent< scene::ecs::TransformComponent >().Translation;
        desc.position = utils::ConvertToPhysXExtendedVector( translation );

        // TODO: Make centralized storage for PxMaterials
        desc.material =
            PhysicsEngine::GetPhysics()->createMaterial( characterControllerComponent.pPhysicsMaterial->StaticFriction,
                characterControllerComponent.pPhysicsMaterial->DynamicFriction,
                characterControllerComponent.pPhysicsMaterial->Restitution );

        desc.userData = this;

        m_pController = pControllerManager->createController( desc );
        SM_ASSERT( m_pController, "CharacterController > Failed to create controller" );

        m_pController->getActor()->setName( characterControllerComponent.Name.c_str() );
        m_pController->getActor()->userData = this;

        SetCollisionGroups( characterControllerComponent.CollisionGroups );
        SetCollisionIgnoreGroups( characterControllerComponent.CollisionIgnoreGroups );
    }

    void CharacterController::UpdateTransform()
    {
        scene::ecs::TransformComponent &transform = m_Entity.GetComponent< scene::ecs::TransformComponent >();

        transform.Translation = GetPosition();
    }

    void CharacterController::SetCollisionGroups( const CollisionGroupFlag groups )
    {
        auto &characterControllerComponent = m_Entity.GetComponent< ecs::CharacterControllerComponent >();
        characterControllerComponent.CollisionIgnoreGroups = groups;

        physx::PxFilterData filterData{ static_cast< Uint32 >( characterControllerComponent.CollisionGroups ),
            static_cast< Uint32 >( characterControllerComponent.CollisionIgnoreGroups ),
            0,
            0 };

        if ( !m_pController )
            return;

        const auto pActor = m_pController->getActor();
        const auto shapeCount = pActor->getNbShapes();
        const auto pShapes = new physx::PxShape *[shapeCount];

        const auto pointerCount = pActor->getShapes( pShapes, shapeCount );
        for ( physx::PxU32 i = 0; i < pointerCount; ++i )
        {
            auto pShape = pShapes[i];
            pShape->setSimulationFilterData( filterData );
            pShape->setQueryFilterData( filterData );
        }
        delete[] pShapes;
    }

    void CharacterController::SetCollisionIgnoreGroups( const CollisionGroupFlag ignoreGroups )
    {
        auto &characterControllerComponent = m_Entity.GetComponent< ecs::CharacterControllerComponent >();
        characterControllerComponent.CollisionIgnoreGroups = ignoreGroups;

        physx::PxFilterData filterData{ static_cast< Uint32 >( characterControllerComponent.CollisionGroups ),
            static_cast< Uint32 >( characterControllerComponent.CollisionIgnoreGroups ),
            0,
            0 };

        if ( !m_pController )
            return;

        const auto pActor = m_pController->getActor();
        const auto shapeCount = pActor->getNbShapes();
        const auto pShapes = new physx::PxShape *[shapeCount];

        const auto pointerCount = pActor->getShapes( pShapes, shapeCount );
        for ( physx::PxU32 i = 0; i < pointerCount; ++i )
        {
            auto pShape = pShapes[i];
            pShape->setSimulationFilterData( filterData );
            // TODO: shouldn't the query filter data be set as well?
        }
        delete[] pShapes;
    }
}