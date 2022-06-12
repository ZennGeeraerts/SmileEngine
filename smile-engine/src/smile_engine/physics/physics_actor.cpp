#include "smpch.h"
#include "physics_actor.h"

#include "physics_engine.h"
#include "physics_utils.h"
#include "smile_engine/math/math.h"

#include <PxPhysicsAPI.h>

namespace smile::physics
{
    static void setPhysicsMaterial( const scene::BoxColliderComponent &component, physx::PxMaterial *out_material )
    {
        if ( component.physicsMaterial )
        {
            if ( out_material )
            {
                out_material->release();
                out_material = nullptr;
            }

            out_material = PhysicsEngine::getPhysics()->createMaterial( component.physicsMaterial->staticFriction,
                component.physicsMaterial->dynamicFriction,
                component.physicsMaterial->restitution );
        }
    }

    static void setPhysicsMaterial( const scene::SphereColliderComponent &component, physx::PxMaterial *out_material )
    {
        if ( component.physicsMaterial )
        {
            if ( out_material )
            {
                out_material->release();
                out_material = nullptr;
            }

            out_material = PhysicsEngine::getPhysics()->createMaterial( component.physicsMaterial->staticFriction,
                component.physicsMaterial->dynamicFriction,
                component.physicsMaterial->restitution );
        }
    }

    static void setPhysicsMaterial( const scene::CapsuleColliderComponent &component, physx::PxMaterial *out_material )
    {
        if ( component.physicsMaterial )
        {
            if ( out_material )
            {
                out_material->release();
                out_material = nullptr;
            }

            out_material = PhysicsEngine::getPhysics()->createMaterial( component.physicsMaterial->staticFriction,
                component.physicsMaterial->dynamicFriction,
                component.physicsMaterial->restitution );
        }
    }

    PhysicsActor::PhysicsActor( scene::Entity entity ) : entity{ entity }
    {
        auto &rigidbody_component = entity.getComponent< scene::RigidbodyComponent >();
        if ( rigidbody_component.physicsMaterial )
            physicsMaterial = rigidbody_component.physicsMaterial;
        else
            physicsMaterial = PhysicsEngine::getDefaultPhysicsMaterial();

        physx::PxPhysics *physics = PhysicsEngine::getPhysics();
        physx::PxTransform px_transform = utils::convertToPhysXTransform( entity.getTransform() );

        switch ( rigidbody_component.bodyType )
        {
            case scene::RigidbodyComponent::BodyType::Static:
            {
                physx::PxRigidStatic *rigid_static_actor = physics->createRigidStatic( px_transform );
                rigidActor = rigid_static_actor;
                break;
            }
            case scene::RigidbodyComponent::BodyType::Dynamic:
            {
                const PhysicsSettings &settings = PhysicsEngine::getPhysicsSettings();

                physx::PxRigidDynamic *rigid_dynamic_actor = physics->createRigidDynamic( px_transform );

                rigid_dynamic_actor->setLinearDamping( rigidbody_component.linearDrag );
                rigid_dynamic_actor->setAngularDamping( rigidbody_component.angularDrag );
                rigid_dynamic_actor->setRigidBodyFlag(
                    physx::PxRigidBodyFlag::eKINEMATIC, rigidbody_component.kinematic );
                rigid_dynamic_actor->setRigidBodyFlag( physx::PxRigidBodyFlag::eENABLE_CCD,
                    rigidbody_component.collisionDetectionType ==
                        scene::RigidbodyComponent::CollisionDetectionType::Continuous );

                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidbody_component.lockPositionX );
                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidbody_component.lockPositionY );
                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidbody_component.lockPositionZ );

                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidbody_component.lockRotationX );
                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidbody_component.lockRotationY );
                rigid_dynamic_actor->setRigidDynamicLockFlag(
                    physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidbody_component.lockRotationZ );

                rigid_dynamic_actor->setActorFlag(
                    physx::PxActorFlag::eDISABLE_GRAVITY, rigidbody_component.disableGravity );
                rigid_dynamic_actor->setSolverIterationCounts(
                    settings.solverIterations, settings.solverVelocityIterations );
                physx::PxRigidBodyExt::setMassAndUpdateInertia( *rigid_dynamic_actor, rigidbody_component.mass );

                rigidActor = rigid_dynamic_actor;
                break;
            }
            default:
                rigidActor = nullptr;
                break;
        }

        pxMaterial = physics->createMaterial(
            physicsMaterial->staticFriction, physicsMaterial->dynamicFriction, physicsMaterial->restitution );

        auto &transformComponent = entity.getComponent< scene::TransformComponent >();
        if ( entity.hasComponent< scene::BoxColliderComponent >() )
        {
            scene::BoxColliderComponent &box_collider_component = entity.getComponent< scene::BoxColliderComponent >();
            setPhysicsMaterial( box_collider_component, pxMaterial );
            addBoxCollider( box_collider_component, transformComponent.scale );
        }
        if ( entity.hasComponent< scene::SphereColliderComponent >() )
        {
            scene::SphereColliderComponent &sphere_collider_component =
                entity.getComponent< scene::SphereColliderComponent >();
            setPhysicsMaterial( sphere_collider_component, pxMaterial );
            addSphereCollider( sphere_collider_component, transformComponent.scale );
        }
        if ( entity.hasComponent< scene::CapsuleColliderComponent >() )
        {
            scene::CapsuleColliderComponent &capsule_collider_component =
                entity.getComponent< scene::CapsuleColliderComponent >();
            setPhysicsMaterial( capsule_collider_component, pxMaterial );
            addCapsuleCollider( capsule_collider_component, transformComponent.scale );
        }

        // Set simulation filter data
        physx::PxAllocatorCallback &allocator = PhysicsEngine::getAllocatorCallback();
        physx::PxFilterData filter_data{};
        filter_data.word0 = BIT( 0 );
        filter_data.word1 = BIT( 0 );

        physx::PxU32 shape_count = rigidActor->getNbShapes();
        physx::PxShape **shapes = static_cast< physx::PxShape ** >(
            allocator.allocate( sizeof( physx::PxShape * ) * shape_count, "", "", 0 ) );
        rigidActor->getShapes( shapes, shape_count );

        for ( physx::PxU32 i{}; i < shape_count; ++i )
            shapes[i]->setSimulationFilterData( filter_data );

        allocator.deallocate( shapes );
        rigidActor->userData = &entity;
    }

    PhysicsActor::~PhysicsActor()
    {
    }

    void PhysicsActor::addBoxCollider( const scene::BoxColliderComponent &component, const DirectX::XMFLOAT3 &size )
    {
        DirectX::XMFLOAT3 collider_size = component.size;

        if ( size.x != 0.0f )
            collider_size.x *= size.x;
        if ( size.y != 0.0f )
            collider_size.y *= size.y;
        if ( size.z != 0.0f )
            collider_size.z *= size.z;

        physx::PxBoxGeometry box_geometry =
            physx::PxBoxGeometry( collider_size.x / 2.0f, collider_size.y / 2.0f, collider_size.z / 2.0f );
        physx::PxShape *shape = physx::PxRigidActorExt::createExclusiveShape( *rigidActor, box_geometry, *pxMaterial );

        shape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.trigger );
        shape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.trigger );

        DirectX::XMMATRIX transform_mat =
            DirectX::XMMatrixScaling( 1.f, 1.f, 1.f ) * DirectX::XMMatrixRotationRollPitchYaw( 0.f, 0.f, 0.f ) *
            DirectX::XMMatrixTranslation( component.offset.x, component.offset.y, component.offset.z );
        DirectX::XMFLOAT4X4 transform{};
        DirectX::XMStoreFloat4x4( &transform, transform_mat );

        shape->setLocalPose( utils::convertToPhysXTransform( transform ) );
    }

    void PhysicsActor::addSphereCollider( const scene::SphereColliderComponent &component,
        const DirectX::XMFLOAT3 &size )
    {
        float collider_radius = component.radius;

        if ( size.x != 0.0f )
            collider_radius *= size.x;

        physx::PxSphereGeometry sphere_geometry = physx::PxSphereGeometry( collider_radius );
        physx::PxShape *shape =
            physx::PxRigidActorExt::createExclusiveShape( *rigidActor, sphere_geometry, *pxMaterial );
        shape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.trigger );
        shape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.trigger );
    }

    void PhysicsActor::addCapsuleCollider( const scene::CapsuleColliderComponent &component,
        const DirectX::XMFLOAT3 &size )
    {
        const float radius_scale = std::max( size.x, size.z );

        physx::PxCapsuleGeometry capsule_geometry =
            physx::PxCapsuleGeometry( component.radius * radius_scale, component.height / 2.f * size.y );
        physx::PxShape *shape =
            physx::PxRigidActorExt::createExclusiveShape( *rigidActor, capsule_geometry, *pxMaterial );
        shape->setFlag( physx::PxShapeFlag::eSIMULATION_SHAPE, !component.trigger );
        shape->setFlag( physx::PxShapeFlag::eTRIGGER_SHAPE, component.trigger );
        shape->setLocalPose( physx::PxTransform{ physx::PxQuat{ physx::PxHalfPi, physx::PxVec3{ 0, 0, 1 } } } );
    }

    void PhysicsActor::updateTransform()
    {
        if ( isDynamic() )
        {
            scene::TransformComponent &transform = entity.getComponent< scene::TransformComponent >();
            physx::PxTransform actor_pose = rigidActor->getGlobalPose();
            DirectX::XMFLOAT3 offset{ 0, 0, 0 };

            if ( entity.hasComponent< scene::BoxColliderComponent >() )
            {
                const auto &boxColliderComponent = entity.getComponent< scene::BoxColliderComponent >();
                offset = boxColliderComponent.offset;
            }

            transform.translation = utils::convertToDirectXVector( actor_pose.p ) /* + offset*/;
            transform.rotation = math::quaternionToEuler( utils::convertToDirectXQuat( actor_pose.q ) );
        }
        else
        {
            rigidActor->setGlobalPose( utils::convertToPhysXTransform( entity.getTransform() ) );
        }
    }

    void PhysicsActor::onFixedUpdate( Timestep delta_time )
    {
    }

    void PhysicsActor::rotate( const DirectX::XMFLOAT3 &rotation )
    {
        physx::PxTransform px_transform = rigidActor->getGlobalPose();
        px_transform.q *= ( physx::PxQuat{ rotation.x, { 1, 0, 0 } } * physx::PxQuat{ rotation.y, { 0, 1, 0 } } *
                            physx::PxQuat{ rotation.z, { 0, 0, 1 } } );
        rigidActor->setGlobalPose( px_transform );
    }
}