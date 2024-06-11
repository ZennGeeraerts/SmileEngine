/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physics_actor.h"

#include "physics_engine.h"
#include "physics_utils.h"
#include "smile_engine/core/math/math.h"

#include <PhysX/PxPhysicsAPI.h>

namespace smile::physics
{
    static void SetPhysicsMaterial( const scene::BoxColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.pPhysicsMaterial->StaticFriction,
                component.pPhysicsMaterial->DynamicFriction,
                component.pPhysicsMaterial->Restitution );
        }
    }

    static void SetPhysicsMaterial( const scene::SphereColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.pPhysicsMaterial->StaticFriction,
                component.pPhysicsMaterial->DynamicFriction,
                component.pPhysicsMaterial->Restitution );
        }
    }

    static void SetPhysicsMaterial( const scene::CapsuleColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.pPhysicsMaterial->StaticFriction,
                component.pPhysicsMaterial->DynamicFriction,
                component.pPhysicsMaterial->Restitution );
        }
    }

    PhysicsActor::PhysicsActor( scene::Entity entity ) : m_Entity{ entity }
    {
        auto &rigidbodyComponent = entity.GetComponent< scene::RigidbodyComponent >();
        if ( !rigidbodyComponent.pPhysicsMaterial )
            rigidbodyComponent.pPhysicsMaterial = PhysicsEngine::GetDefaultPhysicsMaterial();

        physx::PxPhysics *pPhysics = PhysicsEngine::GetPhysics();
        physx::PxTransform pxTransform = utils::ConvertToPhysXTransform( entity.GetTransform() );

        switch ( rigidbodyComponent.Type )
        {
            case scene::RigidbodyComponent::BodyType::Static:
            {
                physx::PxRigidStatic *pRigidStaticActor = pPhysics->createRigidStatic( pxTransform );
                m_pRigidActor = pRigidStaticActor;
                break;
            }
            case scene::RigidbodyComponent::BodyType::Dynamic:
            {
                const PhysicsSettings &settings = PhysicsEngine::GetPhysicsSettings();

                physx::PxRigidDynamic *pRigidDynamicActor = pPhysics->createRigidDynamic( pxTransform );

                pRigidDynamicActor->setLinearDamping( rigidbodyComponent.LinearDrag );
                pRigidDynamicActor->setAngularDamping( rigidbodyComponent.AngularDrag );
                pRigidDynamicActor->setRigidBodyFlag(
                    physx::PxRigidBodyFlag::eKINEMATIC, rigidbodyComponent.IsKinematic );
                pRigidDynamicActor->setRigidBodyFlag( physx::PxRigidBodyFlag::eENABLE_CCD,
                    rigidbodyComponent.CollisionDetection ==
                        scene::RigidbodyComponent::CollisionDetectionType::Continuous );

                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidbodyComponent.LockPositionX );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidbodyComponent.LockPositionY );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidbodyComponent.LockPositionZ );

                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidbodyComponent.LockRotationX );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidbodyComponent.LockRotationY );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidbodyComponent.LockRotationZ );

                pRigidDynamicActor->setActorFlag(
                    physx::PxActorFlag::eDISABLE_GRAVITY, rigidbodyComponent.DisableGravity );
                pRigidDynamicActor->setSolverIterationCounts(
                    settings.SolverIterations, settings.SolverVelocityIterations );
                physx::PxRigidBodyExt::setMassAndUpdateInertia( *pRigidDynamicActor, rigidbodyComponent.Mass );

                m_pRigidActor = pRigidDynamicActor;
                break;
            }
            default:
                m_pRigidActor = nullptr;
                break;
        }

        m_pPxMaterial = pPhysics->createMaterial( rigidbodyComponent.pPhysicsMaterial->StaticFriction,
            rigidbodyComponent.pPhysicsMaterial->DynamicFriction,
            rigidbodyComponent.pPhysicsMaterial->Restitution );

        auto &transformComponent = entity.GetComponent< scene::TransformComponent >();
        if ( entity.HasComponent< scene::BoxColliderComponent >() )
        {
            scene::BoxColliderComponent &boxColliderComponent = entity.GetComponent< scene::BoxColliderComponent >();
            SetPhysicsMaterial( boxColliderComponent, m_pPxMaterial );
            AddBoxCollider( boxColliderComponent, transformComponent.Scale );
        }
        if ( entity.HasComponent< scene::SphereColliderComponent >() )
        {
            scene::SphereColliderComponent &sphereColliderComponent =
                entity.GetComponent< scene::SphereColliderComponent >();
            SetPhysicsMaterial( sphereColliderComponent, m_pPxMaterial );
            AddSphereCollider( sphereColliderComponent, transformComponent.Scale );
        }
        if ( entity.HasComponent< scene::CapsuleColliderComponent >() )
        {
            scene::CapsuleColliderComponent &capsuleColliderComponent =
                entity.GetComponent< scene::CapsuleColliderComponent >();
            SetPhysicsMaterial( capsuleColliderComponent, m_pPxMaterial );
            AddCapsuleCollider( capsuleColliderComponent, transformComponent.Scale );
        }

        // Set simulation filter data
        physx::PxAllocatorCallback &allocator = PhysicsEngine::GetAllocatorCallback();
        physx::PxFilterData filterData{};
        filterData.word0 = BIT( 0 );
        filterData.word1 = BIT( 0 );

        physx::PxU32 shapeCount = m_pRigidActor->getNbShapes();
        physx::PxShape **ppShapes = static_cast< physx::PxShape ** >(
            allocator.allocate( sizeof( physx::PxShape * ) * shapeCount, "", "", 0 ) );
        m_pRigidActor->getShapes( ppShapes, shapeCount );

        for ( physx::PxU32 i{}; i < shapeCount; ++i )
            ppShapes[i]->setSimulationFilterData( filterData );

        allocator.deallocate( ppShapes );
        m_pRigidActor->userData = &entity;
    }

    PhysicsActor::~PhysicsActor()
    {
        if ( m_pRigidActor )
            m_pRigidActor->release();
    }

    void PhysicsActor::AddBoxCollider( const scene::BoxColliderComponent &component, const DirectX::XMFLOAT3 &size )
    {
        DirectX::XMFLOAT3 colliderSize = component.Size;

        if ( size.x != 0.0f )
            colliderSize.x *= size.x;
        if ( size.y != 0.0f )
            colliderSize.y *= size.y;
        if ( size.z != 0.0f )
            colliderSize.z *= size.z;

        physx::PxBoxGeometry boxGeometry =
            physx::PxBoxGeometry( colliderSize.x / 2.0f, colliderSize.y / 2.0f, colliderSize.z / 2.0f );
        physx::PxShape *pShape =
            physx::PxRigidActorExt::createExclusiveShape( *m_pRigidActor, boxGeometry, *m_pPxMaterial );

        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger );

        DirectX::XMMATRIX transformMat =
            DirectX::XMMatrixScaling( 1.f, 1.f, 1.f ) * DirectX::XMMatrixRotationRollPitchYaw( 0.f, 0.f, 0.f ) *
            DirectX::XMMatrixTranslation( component.Offset.x, component.Offset.y, component.Offset.z );
        DirectX::XMFLOAT4X4 transform{};
        DirectX::XMStoreFloat4x4( &transform, transformMat );

        pShape->setLocalPose( utils::ConvertToPhysXTransform( transform ) );
    }

    void PhysicsActor::AddSphereCollider( const scene::SphereColliderComponent &component,
        const DirectX::XMFLOAT3 &size )
    {
        float colliderRadius = component.Radius;

        if ( size.x != 0.0f )
            colliderRadius *= size.x;

        physx::PxSphereGeometry sphereGeometry = physx::PxSphereGeometry( colliderRadius );
        physx::PxShape *pShape =
            physx::PxRigidActorExt::createExclusiveShape( *m_pRigidActor, sphereGeometry, *m_pPxMaterial );
        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger );
    }

    void PhysicsActor::AddCapsuleCollider( const scene::CapsuleColliderComponent &component,
        const DirectX::XMFLOAT3 &size )
    {
        const float radiusScale = std::max( size.x, size.z );

        physx::PxCapsuleGeometry capsuleGeometry =
            physx::PxCapsuleGeometry( component.Radius * radiusScale, component.Height / 2.f * size.y );
        physx::PxShape *pShape =
            physx::PxRigidActorExt::createExclusiveShape( *m_pRigidActor, capsuleGeometry, *m_pPxMaterial );
        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.IsTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.IsTrigger );
        pShape->setLocalPose( physx::PxTransform{ physx::PxQuat{ physx::PxHalfPi, physx::PxVec3{ 0, 0, 1 } } } );
    }

    void PhysicsActor::UpdateTransform()
    {
        if ( IsDynamic() )
        {
            scene::TransformComponent &transform = m_Entity.GetComponent< scene::TransformComponent >();
            physx::PxTransform actorPose = m_pRigidActor->getGlobalPose();
            DirectX::XMFLOAT3 offset{ 0, 0, 0 };

            if ( m_Entity.HasComponent< scene::BoxColliderComponent >() )
            {
                const auto &boxColliderComponent = m_Entity.GetComponent< scene::BoxColliderComponent >();
                offset = boxColliderComponent.Offset;
            }

            transform.Translation = utils::ConvertToDirectXVector( actorPose.p ) /* + offset*/;

            DirectX::XMFLOAT4 rotationQuaternion = utils::ConvertToDirectXQuat( actorPose.q );
            transform.Rotation = math::QuaternionToEuler( rotationQuaternion );
        }
        else
        {
            m_pRigidActor->setGlobalPose( utils::ConvertToPhysXTransform( m_Entity.GetTransform() ) );
        }
    }

    void PhysicsActor::OnFixedUpdate( primitive::Timestep delta_time )
    {
    }

    void PhysicsActor::Translate( const DirectX::XMFLOAT3 &translation )
    {
        if ( !m_pRigidActor || !IsDynamic() )
            return;

        physx::PxTransform pxTransform = m_pRigidActor->getGlobalPose();
        pxTransform.p = utils::ConvertToPhysXVector( translation );
        pxTransform.q = utils::ConvertToPhysXQuat( GetRotation() );

        if ( !IsKinematic() )
            m_pRigidActor->setGlobalPose( pxTransform );
        else
            reinterpret_cast< physx::PxRigidDynamic * >( m_pRigidActor )->setKinematicTarget( pxTransform );
    }

    void PhysicsActor::Rotate( const DirectX::XMFLOAT3 &rotation )
    {
        if ( !m_pRigidActor || !IsDynamic() )
            return;

        physx::PxTransform pxTransform = m_pRigidActor->getGlobalPose();
        pxTransform.p = utils::ConvertToPhysXVector( GetPosition() );
        pxTransform.q = ( physx::PxQuat{ rotation.x, { 1, 0, 0 } } * physx::PxQuat{ rotation.y, { 0, 1, 0 } } *
                          physx::PxQuat{ rotation.z, { 0, 0, 1 } } );

        if ( !IsKinematic() )
            m_pRigidActor->setGlobalPose( pxTransform );
        else
            reinterpret_cast< physx::PxRigidDynamic * >( m_pRigidActor )->setKinematicTarget( pxTransform );
    }

    DirectX::XMFLOAT3 PhysicsActor::GetPosition() const
    {
        physx::PxTransform pxTransform{};
        if ( !IsKinematic() ||
             !reinterpret_cast< physx::PxRigidDynamic * >( m_pRigidActor )->getKinematicTarget( pxTransform ) )
            pxTransform = m_pRigidActor->getGlobalPose();

        return DirectX::XMFLOAT3{ pxTransform.p.x, pxTransform.p.y, pxTransform.p.z };
    }

    DirectX::XMFLOAT4 PhysicsActor::GetRotation() const
    {
        physx::PxTransform pxTransform{};
        if ( !IsKinematic() ||
             !reinterpret_cast< physx::PxRigidDynamic * >( m_pRigidActor )->getKinematicTarget( pxTransform ) )
            pxTransform = m_pRigidActor->getGlobalPose();

        return DirectX::XMFLOAT4{ pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w };
    }

    void PhysicsActor::AddForce( const DirectX::XMFLOAT3 &force, bool autoAwake )
    {
        if ( !m_pRigidActor || !IsDynamic() || IsKinematic() )
        {
            SM_LOG_ERROR( "PhysicsActor::AddForce > Cannot add a force to a static or kinematic actor" );
            return;
        }

        auto pDynamicActor = static_cast< physx::PxRigidDynamic * >( m_pRigidActor );
        pDynamicActor->addForce( utils::ConvertToPhysXVector( force ), physx::PxForceMode::eFORCE, autoAwake );
    }

    void PhysicsActor::AddTorque( const DirectX::XMFLOAT3 &torque, bool autoAwake )
    {
        if ( !m_pRigidActor || !IsDynamic() || IsKinematic() )
        {
            SM_LOG_ERROR( "PhysicsActor::AddTorque > Cannot add a torque to a static or kinematic actor" );
            return;
        }

        auto pDynamicActor = static_cast< physx::PxRigidDynamic * >( m_pRigidActor );
        pDynamicActor->addTorque( utils::ConvertToPhysXVector( torque ), physx::PxForceMode::eFORCE, autoAwake );
    }
}