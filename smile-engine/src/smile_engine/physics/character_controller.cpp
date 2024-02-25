/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "character_controller.h"

#include "physics_engine.h"
#include "physics_utils.h"

#include "smile_engine/scene/components.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    static physx::PxCapsuleClimbingMode::Enum ClimbingModeToPhysXType(
        scene::CharacterControllerComponent::ClimbingModeType climbingMode )
    {
        switch ( climbingMode )
        {
            case scene::CharacterControllerComponent::ClimbingModeType::Easy:
                return physx::PxCapsuleClimbingMode::Enum::eEASY;
            case scene::CharacterControllerComponent::ClimbingModeType::Constrained:
                return physx::PxCapsuleClimbingMode::Enum::eCONSTRAINED;
            case scene::CharacterControllerComponent::ClimbingModeType::Last:
                return physx::PxCapsuleClimbingMode::Enum::eLAST;
        }
    }

    // TODO: Change function name to be shorter :)
    static scene::CharacterControllerComponent::CollisionFlag
    PhysXCharacterControllerCollisionFlagsToCharacterControllerCollisionFlagsType(
        physx::PxControllerCollisionFlags physxCollisionFlags )
    {
        Uint8 collisionFlags{};

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_SIDES ) )
            collisionFlags |= static_cast< Uint8 >( scene::CharacterControllerComponent::CollisionFlag::Sides );

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_UP ) )
            collisionFlags |= static_cast< Uint8 >( scene::CharacterControllerComponent::CollisionFlag::Up );

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_DOWN ) )
            collisionFlags |= static_cast< Uint8 >( scene::CharacterControllerComponent::CollisionFlag::Down );

        return static_cast< scene::CharacterControllerComponent::CollisionFlag >( collisionFlags );
    }

    CharacterController::CharacterController( scene::Entity entity ) : m_Entity{ entity }
    {
        auto pControllerManager = PhysicsEngine::GetControllerManager();

        auto &characterControllerComponent = entity.GetComponent< scene::CharacterControllerComponent >();
        if ( !characterControllerComponent.pPhysicsMaterial )
            characterControllerComponent.pPhysicsMaterial = PhysicsEngine::GetDefaultPhysicsMaterial();

        physx::PxCapsuleControllerDesc desc;
        desc.setToDefault();
        desc.radius = characterControllerComponent.Radius;
        desc.height = characterControllerComponent.Height;
        desc.climbingMode = ClimbingModeToPhysXType( characterControllerComponent.ClimbingMode );
        desc.upDirection = physx::PxVec3{ 0, 1, 0 };
        desc.contactOffset = 0.1f;

        const auto translation = entity.GetComponent< scene::TransformComponent >().Translation;
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
        scene::TransformComponent &transform = m_Entity.GetComponent< scene::TransformComponent >();

        transform.Translation = GetPosition();
    }

    void CharacterController::Translate( const DirectX::XMFLOAT3 &translation )
    {
        SM_ASSERT( m_pController, "CharacterController::Translate > Controller is null" );

        m_pController->setPosition( utils::ConvertToPhysXExtendedVector( translation ) );
    }

    void CharacterController::Move( const DirectX::XMFLOAT3 &displacement, float minDist )
    {
        SM_ASSERT( m_pController, "CharacterController::Move > Controller is null" );

        auto physxControllerCollisionFlags =
            m_pController->move( utils::ConvertToPhysXVector( displacement ), minDist, 0, nullptr, nullptr );

        auto &characterControllerComponent = m_Entity.GetComponent< scene::CharacterControllerComponent >();

        characterControllerComponent.CollisionFlags =
            PhysXCharacterControllerCollisionFlagsToCharacterControllerCollisionFlagsType(
                physxControllerCollisionFlags );
    }

    void CharacterController::SetCollisionGroups( const CollisionGroupFlag groups )
    {
        auto &characterControllerComponent = m_Entity.GetComponent< scene::CharacterControllerComponent >();
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
        auto &characterControllerComponent = m_Entity.GetComponent< scene::CharacterControllerComponent >();
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

    DirectX::XMFLOAT3 CharacterController::GetPosition() const
    {
        SM_ASSERT( m_pController, "CharacterController::GetPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pController->getPosition() );
    }

    DirectX::XMFLOAT3 CharacterController::GetFootPosition() const
    {
        SM_ASSERT( m_pController, "CharacterController::GetFootPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pController->getFootPosition() );
    }
}