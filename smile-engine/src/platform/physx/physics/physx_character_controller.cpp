/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile_engine/physics/character_controller.h"

#include "smile_engine/physics/physics_world.h"
#include "smile_engine/physics/physics_material.h"

#include "physx_utils.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    struct CharacterController::Opaque final
    {
        const PhysicsWorld *pPhysicsWorld;
        Ref< PhysicsMaterial > pPhysicsMaterial;
        physx::PxController *pController;
    };

    static physx::PxCapsuleClimbingMode::Enum ClimbingModeToPhysXType(
        CharacterController::ClimbingModeType climbingMode )
    {
        switch ( climbingMode )
        {
            case CharacterController::ClimbingModeType::Easy:
                return physx::PxCapsuleClimbingMode::Enum::eEASY;
            case CharacterController::ClimbingModeType::Constrained:
                return physx::PxCapsuleClimbingMode::Enum::eCONSTRAINED;
            case CharacterController::ClimbingModeType::Last:
                return physx::PxCapsuleClimbingMode::Enum::eLAST;
        }
    }

    static CharacterController::CollisionFlag
    PhysXCharacterControllerCollisionFlagsToCharacterControllerCollisionFlagsType(
        physx::PxControllerCollisionFlags physxCollisionFlags )
    {
        Uint8 collisionFlags{};

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_SIDES ) )
            collisionFlags |= static_cast< Uint8 >( CharacterController::CollisionFlag::Sides );

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_UP ) )
            collisionFlags |= static_cast< Uint8 >( CharacterController::CollisionFlag::Up );

        if ( physxCollisionFlags.isSet( physx::PxControllerCollisionFlag::eCOLLISION_DOWN ) )
            collisionFlags |= static_cast< Uint8 >( CharacterController::CollisionFlag::Down );

        return static_cast< CharacterController::CollisionFlag >( collisionFlags );
    }

    CharacterController::CharacterController( const PhysicsWorld *pPhysicsWorld,
        float radius,
        float height,
        ClimbingModeType climbingMode,
        const DirectX::XMFLOAT3 &initialTranslation )
    {
        m_pImplementation->pPhysicsWorld = pPhysicsWorld;
        m_pImplementation->pPhysicsMaterial = m_pImplementation->pPhysicsWorld->GetSettings().pDefaultPhysicsMaterial;

        auto pControllerManager = reinterpret_cast< physx::PxControllerManager * >(
            m_pImplementation->pPhysicsWorld->GetControllerManager() );

        physx::PxCapsuleControllerDesc desc;
        desc.setToDefault();
        desc.radius = radius;
        desc.height = height;
        desc.climbingMode = ClimbingModeToPhysXType( climbingMode );
        desc.upDirection = physx::PxVec3{ 0, 1, 0 };
        desc.contactOffset = 0.1f;

        desc.position = utils::ConvertToPhysXExtendedVector( initialTranslation );
        desc.material = reinterpret_cast< physx::PxMaterial * >( m_pImplementation->pPhysicsMaterial->GetInternal() );
        desc.userData = this;

        m_pImplementation->pController = pControllerManager->createController( desc );
        SM_ASSERT( m_pImplementation->pController, "CharacterController > Failed to create controller" );

        m_pImplementation->pController->getActor()->userData = this;
    }

    CharacterController::~CharacterController()
    {
        if ( m_pImplementation->pController && m_pImplementation->pPhysicsWorld &&
             m_pImplementation->pPhysicsWorld->GetInternal() )
        {
            m_pImplementation->pController->release();
            m_pImplementation->pController = nullptr;
        }
    }

    void CharacterController::Translate( const DirectX::XMFLOAT3 &translation )
    {
        SM_ASSERT( m_pImplementation->pController, "CharacterController::Translate > Controller is null" );

        m_pImplementation->pController->setPosition( utils::ConvertToPhysXExtendedVector( translation ) );
    }

    CharacterController::CollisionFlag CharacterController::Move( const DirectX::XMFLOAT3 &displacement, float minDist )
    {
        SM_ASSERT( m_pImplementation->pController, "CharacterController::Move > Controller is null" );

        auto physxControllerCollisionFlags = m_pImplementation->pController->move(
            utils::ConvertToPhysXVector( displacement ), minDist, 0, nullptr, nullptr );

        return PhysXCharacterControllerCollisionFlagsToCharacterControllerCollisionFlagsType(
            physxControllerCollisionFlags );
    }

    void CharacterController::SetCollisionGroups( const CollisionGroupFlag groups,
        const CollisionGroupFlag ignoreGroups )
    {
        physx::PxFilterData filterData{ static_cast< Uint32 >( groups ), static_cast< Uint32 >( ignoreGroups ), 0, 0 };

        SM_ASSERT( !m_pImplementation->pController, "CharacterController::SetCollisionGroups > Controller is null" );

        const auto pActor = m_pImplementation->pController->getActor();
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

    void CharacterController::SetName( const std::string &name )
    {
        m_pImplementation->pController->getActor()->setName( name.c_str() );
    }

    DirectX::XMFLOAT3 CharacterController::GetPosition() const
    {
        SM_ASSERT( m_pImplementation->pController, "CharacterController::GetPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pImplementation->pController->getPosition() );
    }

    DirectX::XMFLOAT3 CharacterController::GetFootPosition() const
    {
        SM_ASSERT( m_pImplementation->pController, "CharacterController::GetFootPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pImplementation->pController->getFootPosition() );
    }
}