/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physx_character_controller.h"
#include "physx_utils.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
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

    PhysXCharacterController::PhysXCharacterController( physx::PxController *pController )
        : m_pController{ pController }
    {
    }

    PhysXCharacterController::~PhysXCharacterController()
    {
        m_pController->release();
    }

    void PhysXCharacterController::Translate( const DirectX::XMFLOAT3 &translation )
    {
        SM_ASSERT( m_pController, "CharacterController::Translate > Controller is null" );

        m_pController->setPosition( utils::ConvertToPhysXExtendedVector( translation ) );
    }

    CharacterController::CollisionFlag PhysXCharacterController::Move( const DirectX::XMFLOAT3 &displacement,
        float minDist )
    {
        SM_ASSERT( m_pController, "CharacterController::Move > Controller is null" );

        auto physxControllerCollisionFlags =
            m_pController->move( utils::ConvertToPhysXVector( displacement ), minDist, 0, nullptr, nullptr );

        return PhysXCharacterControllerCollisionFlagsToCharacterControllerCollisionFlagsType(
            physxControllerCollisionFlags );
    }

    DirectX::XMFLOAT3 PhysXCharacterController::GetPosition() const
    {
        SM_ASSERT( m_pController, "CharacterController::GetPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pController->getPosition() );
    }

    DirectX::XMFLOAT3 PhysXCharacterController::GetFootPosition() const
    {
        SM_ASSERT( m_pController, "CharacterController::GetFootPosition > Controller is null" );

        return utils::ConvertToDirectXVector( m_pController->getFootPosition() );
    }
}