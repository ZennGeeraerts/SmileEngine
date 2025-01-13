/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "engine/physics/physics_shape.h"

#include "engine/physics/rigidbody.h"
#include "physx_utils.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    struct PhysicsShape::Opaque
    {
        const Rigidbody *pRigidbody;
        Ref< PhysicsMaterial > pPhysicsMaterial = nullptr;
        physx::PxShape *pShape = nullptr;
    };

    PhysicsShape::PhysicsShape( const Rigidbody *pRigidbody,
        const PhysicsGeometry *pGeometry,
        Ref< PhysicsMaterial > pPhysicsMaterial )
    {
        m_pImplementation->pRigidbody = pRigidbody;
        m_pImplementation->pPhysicsMaterial = pPhysicsMaterial;

        switch ( pGeometry->Type )
        {
            case PhysicsGeometryType::Box:
            {
                const auto boxGeometry = *static_cast< const PhysicsBoxGeometry * >( pGeometry );
                physx::PxBoxGeometry pxBoxGeometry = physx::PxBoxGeometry{
                    boxGeometry.Box.Size.x, boxGeometry.Box.Size.y, boxGeometry.Box.Size.z };

                auto pRigidActor =
                    reinterpret_cast< physx::PxRigidActor * >( m_pImplementation->pRigidbody->GetInternal() );
                auto pPxMaterial =
                    reinterpret_cast< physx::PxMaterial * >( m_pImplementation->pPhysicsMaterial->GetInternal() );

                m_pImplementation->pShape =
                    physx::PxRigidActorExt::createExclusiveShape( *pRigidActor, pxBoxGeometry, *pPxMaterial );
                SM_ASSERT( m_pImplementation->pShape, "PhysicsShape::PhysicsShape > Failed to create PhysX Shape" );

                DirectX::XMMATRIX transformMat =
                    DirectX::XMMatrixScaling( 1.f, 1.f, 1.f ) * DirectX::XMMatrixRotationRollPitchYaw( 0.f, 0.f, 0.f ) *
                    DirectX::XMMatrixTranslation(
                        boxGeometry.Box.Center.x, boxGeometry.Box.Center.y, boxGeometry.Box.Center.z );
                DirectX::XMFLOAT4X4 transform{};
                DirectX::XMStoreFloat4x4( &transform, transformMat );

                m_pImplementation->pShape->setLocalPose( utils::ConvertToPhysXTransform( transform ) );
                break;
            }
            case PhysicsGeometryType::Sphere:
            {
                const auto sphereGeometry = *static_cast< const PhysicsSphereGeometry * >( pGeometry );
                physx::PxSphereGeometry pxSphereGeometry = physx::PxSphereGeometry{ sphereGeometry.Sphere.Radius };

                auto pRigidActor =
                    reinterpret_cast< physx::PxRigidActor * >( m_pImplementation->pRigidbody->GetInternal() );
                auto pPxMaterial =
                    reinterpret_cast< physx::PxMaterial * >( m_pImplementation->pPhysicsMaterial->GetInternal() );
                m_pImplementation->pShape =
                    physx::PxRigidActorExt::createExclusiveShape( *pRigidActor, pxSphereGeometry, *pPxMaterial );
                // TODO: Add way to offset sphere colliders
                break;
            }
            case PhysicsGeometryType::Capsule:
            {
                const auto capsuleGeometry = *static_cast< const PhysicsCapsuleGeometry * >( pGeometry );

                physx::PxCapsuleGeometry pxCapsuleGeometry =
                    physx::PxCapsuleGeometry{ capsuleGeometry.Capsule.Radius, capsuleGeometry.Capsule.Height };

                auto pRigidActor =
                    reinterpret_cast< physx::PxRigidActor * >( m_pImplementation->pRigidbody->GetInternal() );
                auto pPxMaterial =
                    reinterpret_cast< physx::PxMaterial * >( m_pImplementation->pPhysicsMaterial->GetInternal() );
                m_pImplementation->pShape =
                    physx::PxRigidActorExt::createExclusiveShape( *pRigidActor, pxCapsuleGeometry, *pPxMaterial );

                m_pImplementation->pShape->setLocalPose(
                    physx::PxTransform{ physx::PxQuat{ physx::PxHalfPi, physx::PxVec3{ 0, 0, 1 } } } );
                break;
            }
            case PhysicsGeometryType::Undefined:
            default:
                SM_ASSERT( false, "PhysicsShape > Unsupported shape" )
                break;
        }

        physx::PxFilterData filterData{};
        filterData.word0 = BIT( 0 );
        filterData.word1 = BIT( 0 );
        m_pImplementation->pShape->setSimulationFilterData( filterData );
        m_pImplementation->pShape->userData = this;
    }

    PhysicsShape::~PhysicsShape()
    {
        if ( m_pImplementation->pShape && m_pImplementation->pRigidbody &&
             m_pImplementation->pRigidbody->GetInternal() )
        {
            auto pRigidActor = static_cast< physx::PxRigidActor * >( m_pImplementation->pRigidbody->GetInternal() );
            pRigidActor->detachShape( *m_pImplementation->pShape );
        }
    }

    void PhysicsShape::SetTrigger( bool isTrigger )
    {
        m_pImplementation->pShape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger );
        m_pImplementation->pShape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger );
    }
}