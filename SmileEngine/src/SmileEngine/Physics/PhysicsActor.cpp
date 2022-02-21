#include "smpch.h"
#include "PhysicsActor.h"

#include "PhysicsEngine.h"
#include "PhysicsUtils.h"
#include "SmileEngine/Math/Math.h"

#include <PxPhysicsAPI.h>

namespace smile
{
    static void SetPhysicsMaterial( const BoxColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.m_pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.m_pPhysicsMaterial->StaticFriction,
                component.m_pPhysicsMaterial->DynamicFriction,
                component.m_pPhysicsMaterial->Restitution );
        }
    }

    static void SetPhysicsMaterial( const SphereColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.m_pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.m_pPhysicsMaterial->StaticFriction,
                component.m_pPhysicsMaterial->DynamicFriction,
                component.m_pPhysicsMaterial->Restitution );
        }
    }

    static void SetPhysicsMaterial( const CapsuleColliderComponent &component, physx::PxMaterial *pOutMaterial )
    {
        if ( component.m_pPhysicsMaterial )
        {
            if ( pOutMaterial )
            {
                pOutMaterial->release();
                pOutMaterial = nullptr;
            }

            pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial( component.m_pPhysicsMaterial->StaticFriction,
                component.m_pPhysicsMaterial->DynamicFriction,
                component.m_pPhysicsMaterial->Restitution );
        }
    }

    PhysicsActor::PhysicsActor( Entity entity ) : m_Entity{ entity }
    {
        auto &rigidBodyComponent = entity.GetComponent< RigidbodyComponent >();
        if ( rigidBodyComponent.m_pPhysicsMaterial )
            m_pPhysicsMaterial = rigidBodyComponent.m_pPhysicsMaterial;
        else
            m_pPhysicsMaterial = PhysicsEngine::GetDefaultPhysicsMaterial();

        physx::PxPhysics *pPhysics = PhysicsEngine::GetPhysics();
        physx::PxTransform pxTransform = utils::ConvertToPhysXTransform( entity.GetTransform() );

        switch ( rigidBodyComponent.m_BodyType )
        {
            case RigidbodyComponent::BodyType::Static:
            {
                physx::PxRigidStatic *pRigidStaticActor = pPhysics->createRigidStatic( pxTransform );
                m_pRigidActor = pRigidStaticActor;
                break;
            }
            case RigidbodyComponent::BodyType::Dynamic:
            {
                const PhysicsSettings &settings = PhysicsEngine::GetPhysicsSettings();

                physx::PxRigidDynamic *pRigidDynamicActor = pPhysics->createRigidDynamic( pxTransform );

                pRigidDynamicActor->setLinearDamping( rigidBodyComponent.m_LinearDrag );
                pRigidDynamicActor->setAngularDamping( rigidBodyComponent.m_AngularDrag );
                pRigidDynamicActor->setRigidBodyFlag(
                    physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.m_bKinematic );
                pRigidDynamicActor->setRigidBodyFlag( physx::PxRigidBodyFlag::eENABLE_CCD,
                    rigidBodyComponent.m_CollisionDetectionType == RigidbodyComponent::CollisionDetectionType::Continuous );

                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.m_bLockPositionX );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.m_bLockPositionY );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.m_bLockPositionZ );

                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.m_bLockRotationX );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidBodyComponent.m_bLockRotationY );
                pRigidDynamicActor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidBodyComponent.m_bLockRotationZ );

                pRigidDynamicActor->setActorFlag(
                    physx::PxActorFlag::eDISABLE_GRAVITY, rigidBodyComponent.m_bDisableGravity );
                pRigidDynamicActor->setSolverIterationCounts(
                    settings.m_SolverIterations, settings.m_SolverVelocityIterations );
                physx::PxRigidBodyExt::setMassAndUpdateInertia( *pRigidDynamicActor, rigidBodyComponent.m_Mass );

                m_pRigidActor = pRigidDynamicActor;
                break;
            }
        }

        m_pPxMaterial = pPhysics->createMaterial(
            m_pPhysicsMaterial->StaticFriction, m_pPhysicsMaterial->DynamicFriction, m_pPhysicsMaterial->Restitution );

        auto &transformComponent = entity.GetComponent< TransformComponent >();
        if ( entity.HasComponent< BoxColliderComponent >() )
        {
            BoxColliderComponent &boxColliderComponent = entity.GetComponent< BoxColliderComponent >();
            SetPhysicsMaterial( boxColliderComponent, m_pPxMaterial );
            AddBoxCollider( boxColliderComponent, transformComponent.m_Scale );
        }
        if ( entity.HasComponent< SphereColliderComponent >() )
        {
            SphereColliderComponent &sphereColliderComponent = entity.GetComponent< SphereColliderComponent >();
            SetPhysicsMaterial( sphereColliderComponent, m_pPxMaterial );
            AddSphereCollider( sphereColliderComponent, transformComponent.m_Scale );
        }
        if ( entity.HasComponent< CapsuleColliderComponent >() )
        {
            CapsuleColliderComponent &capsuleColliderComponent = entity.GetComponent< CapsuleColliderComponent >();
            SetPhysicsMaterial( capsuleColliderComponent, m_pPxMaterial );
            AddCapsuleCollider( capsuleColliderComponent, transformComponent.m_Scale );
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
    }

    void PhysicsActor::AddBoxCollider( const BoxColliderComponent &component, const DirectX::XMFLOAT3 &size )
    {
        DirectX::XMFLOAT3 colliderSize = component.m_Size;

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

        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.m_bTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.m_bTrigger );

        DirectX::XMMATRIX transformMat =
            DirectX::XMMatrixScaling( 1.f, 1.f, 1.f ) * DirectX::XMMatrixRotationRollPitchYaw( 0.f, 0.f, 0.f ) *
            DirectX::XMMatrixTranslation( component.m_Offset.x, component.m_Offset.y, component.m_Offset.z );
        DirectX::XMFLOAT4X4 transform{};
        DirectX::XMStoreFloat4x4( &transform, transformMat );

        pShape->setLocalPose( utils::ConvertToPhysXTransform( transform ) );
    }

    void PhysicsActor::AddSphereCollider( const SphereColliderComponent &component, const DirectX::XMFLOAT3 &size )
    {
        float colliderRadius = component.m_Radius;

        if ( size.x != 0.0f )
            colliderRadius *= size.x;

        physx::PxSphereGeometry sphereGeometry = physx::PxSphereGeometry( colliderRadius );
        physx::PxShape *pShape =
            physx::PxRigidActorExt::createExclusiveShape( *m_pRigidActor, sphereGeometry, *m_pPxMaterial );
        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.m_bTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.m_bTrigger );
    }

    void PhysicsActor::AddCapsuleCollider( const CapsuleColliderComponent &component, const DirectX::XMFLOAT3 &size )
    {
        float radiusScale = std::max( size.x, size.z );

        physx::PxCapsuleGeometry capsuleGeometry =
            physx::PxCapsuleGeometry( component.m_Radius * radiusScale, component.m_Height / 2.f * size.y );
        physx::PxShape *pShape =
            physx::PxRigidActorExt::createExclusiveShape( *m_pRigidActor, capsuleGeometry, *m_pPxMaterial );
        pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.m_bTrigger );
        pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.m_bTrigger );
        pShape->setLocalPose( physx::PxTransform{ physx::PxQuat{ physx::PxHalfPi, physx::PxVec3{ 0, 0, 1 } } } );
    }

    void PhysicsActor::UpdateTransform()
    {
        if ( IsDynamic() )
        {
            TransformComponent &transform = m_Entity.GetComponent< TransformComponent >();
            physx::PxTransform actorPose = m_pRigidActor->getGlobalPose();
            transform.m_Translation = utils::ConvertToDirectXVector( actorPose.p );
            transform.m_Rotation = Math::QuaternionToEuler( utils::ConvertToDirectXQuat( actorPose.q ) );
        }
        else
        {
            m_pRigidActor->setGlobalPose( utils::ConvertToPhysXTransform( m_Entity.GetTransform() ) );
        }
    }

    void PhysicsActor::OnFixedUpdate( Timestep deltaTime )
    {
    }

    void PhysicsActor::Rotate( const DirectX::XMFLOAT3 &rotation )
    {
        physx::PxTransform pxTransform = m_pRigidActor->getGlobalPose();
        pxTransform.q *= ( physx::PxQuat{ rotation.x, { 1, 0, 0 } } * physx::PxQuat{ rotation.y, { 0, 1, 0 } } *
                           physx::PxQuat{ rotation.z, { 0, 0, 1 } } );
        m_pRigidActor->setGlobalPose( pxTransform );
    }
}