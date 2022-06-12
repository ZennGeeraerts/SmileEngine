#pragma once
#include "smile_engine/scene/entity.h"

namespace physx
{
    class PxRigidActor;
    class PxMaterial;
}

namespace smile::physics
{
    class PhysicsActor final
    {
      public:
        PhysicsActor( scene::Entity entity );
        ~PhysicsActor();

        PhysicsActor( const PhysicsActor & ) = delete;
        PhysicsActor( PhysicsActor && ) = delete;
        PhysicsActor &operator=( const PhysicsActor & ) = delete;
        PhysicsActor &operator=( PhysicsActor && ) = delete;

        void onFixedUpdate( Timestep delta_time );
        void rotate( const DirectX::XMFLOAT3 &rotation );

        bool isDynamic() const
        {
            return entity.getComponent< scene::RigidbodyComponent >().bodyType == scene::RigidbodyComponent::BodyType::Dynamic;
        }

      private:
        void addBoxCollider( const scene::BoxColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void addSphereCollider( const scene::SphereColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void addCapsuleCollider( const scene::CapsuleColliderComponent &component, const DirectX::XMFLOAT3 &size );

        void updateTransform();

      private:
        scene::Entity entity;
        Ref< PhysicsMaterial > physicsMaterial;

        physx::PxRigidActor *rigidActor = nullptr;
        physx::PxMaterial *pxMaterial = nullptr;

        friend class PhysicsEngine;
    };
}