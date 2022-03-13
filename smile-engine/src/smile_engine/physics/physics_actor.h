#pragma once
#include "smile_engine/scene/entity.h"

namespace physx
{
    class PxRigidActor;
    class PxMaterial;
}

namespace smile
{
    class PhysicsActor final
    {
      public:
        PhysicsActor( Entity entity );
        ~PhysicsActor();

        void OnFixedUpdate( Timestep deltaTime );
        void Rotate( const DirectX::XMFLOAT3 &rotation );

        bool IsDynamic() const
        {
            return m_Entity.GetComponent< RigidbodyComponent >().m_BodyType == RigidbodyComponent::BodyType::Dynamic;
        }

      private:
        void AddBoxCollider( const BoxColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddSphereCollider( const SphereColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddCapsuleCollider( const CapsuleColliderComponent &component, const DirectX::XMFLOAT3 &size );

        void UpdateTransform();

      private:
        Entity m_Entity;
        Ref< PhysicsMaterial > m_pPhysicsMaterial;

        physx::PxRigidActor *m_pRigidActor = nullptr;
        physx::PxMaterial *m_pPxMaterial = nullptr;

        friend class PhysicsEngine;
    };
}