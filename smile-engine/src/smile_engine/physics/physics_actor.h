#pragma once
#include "smile_engine/scene/entity.h"

namespace physx
{
    class PxRigidActor;
    class PxMaterial;
}

namespace Smile::Physics
{
    class PhysicsActor final
    {
      public:
        PhysicsActor( Scene::Entity entity );
        ~PhysicsActor();

        PhysicsActor( const PhysicsActor & ) = delete;
        PhysicsActor( PhysicsActor && ) = delete;
        PhysicsActor &operator=( const PhysicsActor & ) = delete;
        PhysicsActor &operator=( PhysicsActor && ) = delete;

        void OnFixedUpdate( Timestep deltaTime );
        void Rotate( const DirectX::XMFLOAT3 &rotation );

        bool IsDynamic() const
        {
            return m_Entity.GetComponent< Scene::RigidbodyComponent >().Type == Scene::RigidbodyComponent::BodyType::Dynamic;
        }

      private:
        void AddBoxCollider( const Scene::BoxColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddSphereCollider( const Scene::SphereColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddCapsuleCollider( const Scene::CapsuleColliderComponent &component, const DirectX::XMFLOAT3 &size );

        void UpdateTransform();

      private:
        Scene::Entity m_Entity;
        Ref< PhysicsMaterial > m_pPhysicsMaterial;

        physx::PxRigidActor *m_pRigidActor = nullptr;
        physx::PxMaterial *m_pPxMaterial = nullptr;

        friend class PhysicsEngine;
    };
}