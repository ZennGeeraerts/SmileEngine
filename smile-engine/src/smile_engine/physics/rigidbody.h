/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/common/compiled/pimpl.h"
#include "physics_shape.h"

#include "ecs/box_collider_component.h"
#include "ecs/sphere_collider_component.h"
#include "ecs/capsule_collider_component.h"

#include <DirectXMath.h>

namespace smile::physics
{
    class PhysicsWorld;

    enum class RigidbodyType
    {
        Static = 0,
        Dynamic = 1
    };

    enum class CollisionDetectionType
    {
        Discrete = 0,
        Continuous = 1
    };

    class Rigidbody final
    {
      private:
        struct Opaque;

      public:
        Rigidbody( const PhysicsWorld *pPhysicsWorld,
            RigidbodyType bodyType,
            const DirectX::XMFLOAT4X4 &initialTransform );
        ~Rigidbody();

        Rigidbody( const Rigidbody & ) = delete;
        Rigidbody( Rigidbody && ) = delete;
        Rigidbody &operator=( const Rigidbody & ) = delete;
        Rigidbody &operator=( Rigidbody && ) = delete;

        Ref< PhysicsShape > CreateShape( const PhysicsGeometry *pGeometry );
        void RemoveShape( Ref< PhysicsShape > pPhysicsShape );

        void SetLinearDrag( float linearDrag );
        float GetLinearDrag() const;

        void SetAngularDrag( float angularDrag );
        float GetAngularDrag() const;

        void SetKinematic( bool isKinematic );
        bool IsKinematic() const;

        void SetCollisionDetection( CollisionDetectionType collisionDetection );
        CollisionDetectionType GetCollisionDetection() const;

        void LockTranslation( bool lockX, bool lockY, bool lockZ );
        void LockRotation( bool lockX, bool lockY, bool lockZ );

        void SetDisableGravity( bool disableGravity );
        bool GetDisableGravity() const;

        void SetMass( float mass );
        float GetMass() const;

        void Translate( const DirectX::XMFLOAT3 &translation );
        void Rotate( const DirectX::XMFLOAT3 &rotation );

        void AddForce( const DirectX::XMFLOAT3 &force, bool autoAwake = true );
        void AddTorque( const DirectX::XMFLOAT3 &torque, bool autoAwake = true );

        DirectX::XMFLOAT3 GetPosition() const;
        DirectX::XMFLOAT4 GetRotation() const;

        bool IsDynamic() const;
        Ref< PhysicsMaterial > GetPhysicsMaterial() const;
        void *GetInternal() const;

      private:
        compiled::PImpl< Opaque > m_pImplementation;
    };
}