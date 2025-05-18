/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/physics/rigidbody.h"

#include "smile/physics/physics_world.h"
#include "smile/physics/physics_engine.h"

#include "physx_utils.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    struct Rigidbody::Opaque final
    {
        const PhysicsWorld *pPhysicsWorld;
        RigidbodyType BodyType = RigidbodyType::Static;
        Ref< PhysicsMaterial > pPhysicsMaterial;
        std::vector< Ref< PhysicsShape > > pShapes{};
        physx::PxRigidActor *pRigidActor = nullptr;
    };

    Rigidbody::Rigidbody( const PhysicsWorld *pPhysicsWorld,
        RigidbodyType bodyType,
        const DirectX::XMFLOAT4X4 &initialTransform )
    {
        m_pImplementation->pPhysicsWorld = pPhysicsWorld;
        m_pImplementation->BodyType = bodyType;
        m_pImplementation->pPhysicsMaterial = m_pImplementation->pPhysicsWorld->GetSettings().pDefaultPhysicsMaterial;

        auto pPhysics = reinterpret_cast< physx::PxPhysics * >( PhysicsEngine::GetInstance().GetPhysics() );
        physx::PxTransform pxTransform = utils::ConvertToPhysXTransform( initialTransform );

        switch ( m_pImplementation->BodyType )
        {
            case RigidbodyType::Static:
            {
                physx::PxRigidStatic *pRigidStaticActor = pPhysics->createRigidStatic( pxTransform );
                m_pImplementation->pRigidActor = pRigidStaticActor;
                break;
            }
            case RigidbodyType::Dynamic:
            {
                const PhysicsWorldSettings &settings = m_pImplementation->pPhysicsWorld->GetSettings();

                physx::PxRigidDynamic *pRigidDynamicActor = pPhysics->createRigidDynamic( pxTransform );
                pRigidDynamicActor->setSolverIterationCounts(
                    settings.SolverIterations, settings.SolverVelocityIterations );
                m_pImplementation->pRigidActor = pRigidDynamicActor;
                break;
            }
            default:
                SM_ASSERT_MSG( false, "Rigidbody::Rigidbody > Unsupported rigidbody type" )
                break;
        }

        m_pImplementation->pRigidActor->userData = this;
    }

    Rigidbody::~Rigidbody()
    {
        m_pImplementation->pShapes.clear();

        if ( m_pImplementation->pRigidActor )
        {
            m_pImplementation->pRigidActor->release();
            m_pImplementation->pRigidActor = nullptr;
        }
    }

    Ref< PhysicsShape > Rigidbody::CreateShape( const PhysicsGeometry *pGeometry )
    {
        auto pShape = CreateRef< PhysicsShape >( this, pGeometry, m_pImplementation->pPhysicsMaterial );
        m_pImplementation->pShapes.emplace_back( pShape );
        return pShape;
    }

    void Rigidbody::RemoveShape( Ref< PhysicsShape > pPhysicsShape )
    {
        m_pImplementation->pShapes.erase(
            std::remove( m_pImplementation->pShapes.begin(), m_pImplementation->pShapes.end(), pPhysicsShape ) );
    }

    void Rigidbody::SetLinearDrag( float linearDrag )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::SetLinearDrag > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setLinearDamping( linearDrag );
    }

    float Rigidbody::GetLinearDrag() const
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::GetLinearDrag > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        return pRigidActorDynamic->getLinearDamping();
    }

    void Rigidbody::SetAngularDrag( float angularDrag )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::SetAngularDrag > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setAngularDamping( angularDrag );
    }

    float Rigidbody::GetAngularDrag() const
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::GetAngularDrag > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        return pRigidActorDynamic->getAngularDamping();
    }

    void Rigidbody::SetKinematic( bool isKinematic )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::SetKinematic > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setRigidBodyFlag( physx::PxRigidBodyFlag::eKINEMATIC, isKinematic );
    }

    bool Rigidbody::IsKinematic() const
    {
        if ( !IsDynamic() )
            return false;

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        return pRigidActorDynamic->getRigidBodyFlags().isSet( physx::PxRigidBodyFlag::eKINEMATIC );
    }

    void Rigidbody::SetCollisionDetection( CollisionDetectionType collisionDetection )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::SetCollisionDetection > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setRigidBodyFlag(
            physx::PxRigidBodyFlag::eENABLE_CCD, collisionDetection == CollisionDetectionType::Continuous );
    }

    CollisionDetectionType Rigidbody::GetCollisionDetection() const
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::GetCollisionDetection > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        return pRigidActorDynamic->getRigidBodyFlags().isSet( physx::PxRigidBodyFlag::eENABLE_CCD )
                   ? CollisionDetectionType::Continuous
                   : CollisionDetectionType::Discrete;
    }

    void Rigidbody::LockTranslation( bool lockX, bool lockY, bool lockZ )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::LockTranslation > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, lockX );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, lockY );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, lockZ );
    }

    void Rigidbody::LockRotation( bool lockX, bool lockY, bool lockZ )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::LockRotation > Rigidbody type is not dynamic" );

        auto pRigidActorDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockX );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockY );
        pRigidActorDynamic->setRigidDynamicLockFlag( physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockZ );
    }

    void Rigidbody::SetDisableGravity( bool disableGravity )
    {
        m_pImplementation->pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_GRAVITY, disableGravity );
    }

    bool Rigidbody::GetDisableGravity() const
    {
        return m_pImplementation->pRigidActor->getActorFlags().isSet( physx::PxActorFlag::eDISABLE_GRAVITY );
    }

    void Rigidbody::SetMass( float mass )
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::SetMass > Rigidbody type is not dynamic" );

        auto pRigidbody = static_cast< physx::PxRigidBody * >( m_pImplementation->pRigidActor );
        physx::PxRigidBodyExt::setMassAndUpdateInertia( *pRigidbody, mass );
    }

    float Rigidbody::GetMass() const
    {
        SM_ASSERT_MSG( IsDynamic(), "Rigidbody::GetMass > Rigidbody type is not dynamic" );

        auto pRigidbody = static_cast< physx::PxRigidBody * >( m_pImplementation->pRigidActor );
        return pRigidbody->getMass();
    }

    void Rigidbody::Translate( const DirectX::XMFLOAT3 &translation )
    {
        if ( !IsDynamic() )
            return;

        physx::PxTransform pxTransform = m_pImplementation->pRigidActor->getGlobalPose();
        pxTransform.p = utils::ConvertToPhysXVector( translation );
        pxTransform.q = utils::ConvertToPhysXQuat( GetRotation() );

        if ( !IsKinematic() )
        {
            m_pImplementation->pRigidActor->setGlobalPose( pxTransform );
        }
        else
        {
            auto pRigidDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
            pRigidDynamic->setKinematicTarget( pxTransform );
        }
    }

    void Rigidbody::Rotate( const DirectX::XMFLOAT3 &rotation )
    {
        if ( !IsDynamic() )
            return;

        physx::PxTransform pxTransform = m_pImplementation->pRigidActor->getGlobalPose();
        pxTransform.p = utils::ConvertToPhysXVector( GetPosition() );
        pxTransform.q = ( physx::PxQuat{ rotation.x, { 1, 0, 0 } } * physx::PxQuat{ rotation.y, { 0, 1, 0 } } *
                          physx::PxQuat{ rotation.z, { 0, 0, 1 } } );

        if ( !IsKinematic() )
        {
            m_pImplementation->pRigidActor->setGlobalPose( pxTransform );
        }
        else
        {
            auto pRigidDynamic = reinterpret_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
            pRigidDynamic->setKinematicTarget( pxTransform );
        }
    }

    void Rigidbody::AddForce( const DirectX::XMFLOAT3 &force, bool autoAwake )
    {
        if ( !IsDynamic() || IsKinematic() )
        {
            SM_LOG_ERROR( "Rigidbody::AddForce > Cannot add a force to a static or kinematic actor" );
            return;
        }

        auto pRigidDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidDynamic->addForce( utils::ConvertToPhysXVector( force ), physx::PxForceMode::eFORCE, autoAwake );
    }

    void Rigidbody::AddTorque( const DirectX::XMFLOAT3 &torque, bool autoAwake )
    {
        if ( !IsDynamic() || IsKinematic() )
        {
            SM_LOG_ERROR( "Rigidbody::AddTorque > Cannot add a torque to a static or kinematic actor" );
            return;
        }

        auto pRigidDynamic = static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor );
        pRigidDynamic->addTorque( utils::ConvertToPhysXVector( torque ), physx::PxForceMode::eFORCE, autoAwake );
    }

    DirectX::XMFLOAT3 Rigidbody::GetPosition() const
    {
        physx::PxTransform pxTransform{ physx::PxIdentity };
        if ( !IsKinematic() || !static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor )
                                    ->getKinematicTarget( pxTransform ) )
        {
            pxTransform = m_pImplementation->pRigidActor->getGlobalPose();
        }

        return utils::ConvertToDirectXVector( pxTransform.p );
    }

    DirectX::XMFLOAT4 Rigidbody::GetRotation() const
    {
        physx::PxTransform pxTransform{ physx::PxIdentity };
        if ( !IsKinematic() || !static_cast< physx::PxRigidDynamic * >( m_pImplementation->pRigidActor )
                                    ->getKinematicTarget( pxTransform ) )
        {
            pxTransform = m_pImplementation->pRigidActor->getGlobalPose();
        }

        return utils::ConvertToDirectXQuat( pxTransform.q );
    }

    bool Rigidbody::IsDynamic() const
    {
        return m_pImplementation->BodyType == RigidbodyType::Dynamic;
    }

    Ref< PhysicsMaterial > Rigidbody::GetPhysicsMaterial() const
    {
        return m_pImplementation->pPhysicsMaterial;
    }

    void *Rigidbody::GetInternal() const
    {
        return m_pImplementation->pRigidActor;
    }
}