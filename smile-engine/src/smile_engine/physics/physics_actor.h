/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
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

        void OnFixedUpdate( Timestep deltaTime );
        void Translate( const DirectX::XMFLOAT3 &translation );
        void Rotate( const DirectX::XMFLOAT3 &rotation );

        DirectX::XMFLOAT3 GetPosition() const;
        DirectX::XMFLOAT4 GetRotation() const;

        bool IsDynamic() const
        {
            return m_Entity.GetComponent< scene::RigidbodyComponent >().Type == scene::RigidbodyComponent::BodyType::Dynamic;
        }
        bool IsKinematic() const
        {
            return m_Entity.GetComponent< scene::RigidbodyComponent >().IsKinematic;
        }

        void AddForce( const DirectX::XMFLOAT3 &force, bool autoAwake = true );
        void AddTorque( const DirectX::XMFLOAT3 &torque, bool autoAwake = true );

      private:
        void AddBoxCollider( const scene::BoxColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddSphereCollider( const scene::SphereColliderComponent &component, const DirectX::XMFLOAT3 &size );
        void AddCapsuleCollider( const scene::CapsuleColliderComponent &component, const DirectX::XMFLOAT3 &size );

        void UpdateTransform();

      private:
        scene::Entity m_Entity;

        physx::PxRigidActor *m_pRigidActor = nullptr;
        physx::PxMaterial *m_pPxMaterial = nullptr;

        friend class PhysicsEngine;
    };
}