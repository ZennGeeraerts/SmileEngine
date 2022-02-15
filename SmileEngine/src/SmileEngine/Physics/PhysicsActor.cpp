#include "smpch.h"
#include "PhysicsActor.h"

#include "PhysicsEngine.h"
#include "PhysicsUtils.h"
#include "SmileEngine/Math/Math.h"

#include <PxPhysicsAPI.h>

namespace Smile
{
	static void SetPhysicsMaterial(const BoxColliderComponent& component, physx::PxMaterial* pOutMaterial)
	{
		if (component.pPhysicsMaterial)
		{
			if (pOutMaterial)
			{
				pOutMaterial->release();
				pOutMaterial = nullptr;
			}

			pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial(
				component.pPhysicsMaterial->StaticFriction, component.pPhysicsMaterial->DynamicFriction, component.pPhysicsMaterial->Restitution);
		}
	}

	static void SetPhysicsMaterial(const SphereColliderComponent& component, physx::PxMaterial* pOutMaterial)
	{
		if (component.pPhysicsMaterial)
		{
			if (pOutMaterial)
			{
				pOutMaterial->release();
				pOutMaterial = nullptr;
			}

			pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial(
				component.pPhysicsMaterial->StaticFriction, component.pPhysicsMaterial->DynamicFriction, component.pPhysicsMaterial->Restitution);
		}
	}

	static void SetPhysicsMaterial(const CapsuleColliderComponent& component, physx::PxMaterial* pOutMaterial)
	{
		if (component.pPhysicsMaterial)
		{
			if (pOutMaterial)
			{
				pOutMaterial->release();
				pOutMaterial = nullptr;
			}

			pOutMaterial = PhysicsEngine::GetPhysics()->createMaterial(
				component.pPhysicsMaterial->StaticFriction, component.pPhysicsMaterial->DynamicFriction, component.pPhysicsMaterial->Restitution);
		}
	}

	PhysicsActor::PhysicsActor(Entity entity)
		: m_Entity{ entity }
	{
		auto& rigidBodyComponent = entity.GetComponent<RigidbodyComponent>();
		if (rigidBodyComponent.pPhysicsMaterial)
			m_pPhysicsMaterial = rigidBodyComponent.pPhysicsMaterial;
		else
			m_pPhysicsMaterial = PhysicsEngine::GetDefaultPhysicsMaterial();

		physx::PxPhysics* pPhysics = PhysicsEngine::GetPhysics();
		physx::PxTransform pxTransform = PhysicsUtils::ConvertToPhysXTransform(entity.GetTransform());

		switch (rigidBodyComponent.Type)
		{
		case RigidbodyComponent::BodyType::eStatic:
		{
			physx::PxRigidStatic* pRigidStaticActor = pPhysics->createRigidStatic(pxTransform);
			m_pRigidActor = pRigidStaticActor;
			break;
		}
		case RigidbodyComponent::BodyType::eDynamic:
		{
			const PhysicsSettings& settings = PhysicsEngine::GetPhysicsSettings();

			physx::PxRigidDynamic* pRigidDynamicActor = pPhysics->createRigidDynamic(pxTransform);

			pRigidDynamicActor->setLinearDamping(rigidBodyComponent.LinearDrag);
			pRigidDynamicActor->setAngularDamping(rigidBodyComponent.AngularDrag);
			pRigidDynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.bKinematic);
			pRigidDynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, rigidBodyComponent.CollisionDetection == RigidbodyComponent::CollisionDetectionType::eContinuous);

			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.bLockPositionX);
			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.bLockPositionY);
			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.bLockPositionZ);

			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.bLockRotationX);
			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, rigidBodyComponent.bLockRotationY);
			pRigidDynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, rigidBodyComponent.bLockRotationZ);

			pRigidDynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, rigidBodyComponent.bDisableGravity);
			pRigidDynamicActor->setSolverIterationCounts(settings.SolverIterations, settings.SolverVelocityIterations);
			physx::PxRigidBodyExt::setMassAndUpdateInertia(*pRigidDynamicActor, rigidBodyComponent.Mass);

			m_pRigidActor = pRigidDynamicActor;
			break;
		}
		}

		m_pPxMaterial = pPhysics->createMaterial(m_pPhysicsMaterial->StaticFriction, m_pPhysicsMaterial->DynamicFriction, m_pPhysicsMaterial->Restitution);

		auto& transformComponent = entity.GetComponent<TransformComponent>();
		if (entity.HasComponent<BoxColliderComponent>())
		{
			BoxColliderComponent& boxColliderComponent = entity.GetComponent<BoxColliderComponent>();
			SetPhysicsMaterial(boxColliderComponent, m_pPxMaterial);
			AddBoxCollider(boxColliderComponent, transformComponent.Scale);
		}
		if (entity.HasComponent<SphereColliderComponent>())
		{
			SphereColliderComponent& sphereColliderComponent = entity.GetComponent<SphereColliderComponent>();
			SetPhysicsMaterial(sphereColliderComponent, m_pPxMaterial);
			AddSphereCollider(sphereColliderComponent, transformComponent.Scale);
		}
		if (entity.HasComponent<CapsuleColliderComponent>())
		{
			CapsuleColliderComponent& capsuleColliderComponent = entity.GetComponent<CapsuleColliderComponent>();
			SetPhysicsMaterial(capsuleColliderComponent, m_pPxMaterial);
			AddCapsuleCollider(capsuleColliderComponent, transformComponent.Scale);
		}

		// Set simulation filter data
		physx::PxAllocatorCallback& allocator = PhysicsEngine::GetAllocatorCallback();
		physx::PxFilterData filterData{};
		filterData.word0 = BIT(0);
		filterData.word1 = BIT(0);

		physx::PxU32 shapeCount = m_pRigidActor->getNbShapes();
		physx::PxShape** ppShapes = static_cast<physx::PxShape**>(allocator.allocate(sizeof(physx::PxShape*) * shapeCount, "", "", 0));
		m_pRigidActor->getShapes(ppShapes, shapeCount);

		for (physx::PxU32 i{}; i < shapeCount; ++i)
			ppShapes[i]->setSimulationFilterData(filterData);

		allocator.deallocate(ppShapes);
		m_pRigidActor->userData = &entity;
	}

	PhysicsActor::~PhysicsActor()
	{
		
	}

	void PhysicsActor::AddBoxCollider(const BoxColliderComponent& component, const DirectX::XMFLOAT3& size)
	{
		DirectX::XMFLOAT3 colliderSize = component.Size;
		
		if (size.x != 0.0f) colliderSize.x *= size.x;
		if (size.y != 0.0f) colliderSize.y *= size.y;
		if (size.z != 0.0f) colliderSize.z *= size.z;

		physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(colliderSize.x / 2.0f, colliderSize.y / 2.0f, colliderSize.z / 2.0f);
		physx::PxShape* pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidActor, boxGeometry, *m_pPxMaterial);

		pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.bTrigger);
		pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.bTrigger);

		DirectX::XMMATRIX transformMat = DirectX::XMMatrixScaling(1.f, 1.f, 1.f) * DirectX::XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f)
			* DirectX::XMMatrixTranslation(component.Offset.x, component.Offset.y, component.Offset.z);
		DirectX::XMFLOAT4X4 transform{};
		DirectX::XMStoreFloat4x4(&transform, transformMat);

		pShape->setLocalPose(PhysicsUtils::ConvertToPhysXTransform(transform));
	}

	void PhysicsActor::AddSphereCollider(const SphereColliderComponent& component, const DirectX::XMFLOAT3& size)
	{
		float colliderRadius = component.Radius;

		if (size.x != 0.0f) colliderRadius *= size.x;

		physx::PxSphereGeometry sphereGeometry = physx::PxSphereGeometry(colliderRadius);
		physx::PxShape* pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidActor, sphereGeometry, *m_pPxMaterial);
		pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.bTrigger);
		pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.bTrigger);
	}

	void PhysicsActor::AddCapsuleCollider(const CapsuleColliderComponent& component, const DirectX::XMFLOAT3& size)
	{
		float radiusScale = std::max(size.x, size.z);

		physx::PxCapsuleGeometry capsuleGeometry = physx::PxCapsuleGeometry(component.Radius * radiusScale, component.Height / 2.f * size.y);
		physx::PxShape* pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidActor, capsuleGeometry, *m_pPxMaterial);
		pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !component.bTrigger);
		pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, component.bTrigger);
		pShape->setLocalPose(physx::PxTransform{ physx::PxQuat{ physx::PxHalfPi, physx::PxVec3{ 0, 0, 1 } } });
	}

	void PhysicsActor::UpdateTransform()
	{
		if (IsDynamic())
		{
			TransformComponent& transform = m_Entity.GetComponent<TransformComponent>();
			physx::PxTransform actorPose = m_pRigidActor->getGlobalPose();
			transform.Translation = PhysicsUtils::ConvertToDirectXVector(actorPose.p);
			transform.Rotation = Math::QuaternionToEuler(PhysicsUtils::ConvertToDirectXQuat(actorPose.q));
		}
		else
		{
			m_pRigidActor->setGlobalPose(PhysicsUtils::ConvertToPhysXTransform(m_Entity.GetTransform()));
		}
	}

	void PhysicsActor::OnFixedUpdate(Timestep deltaTime)
	{

	}

	void PhysicsActor::Rotate(const DirectX::XMFLOAT3& rotation)
	{
		physx::PxTransform pxTransform = m_pRigidActor->getGlobalPose();
		pxTransform.q *= (physx::PxQuat{ rotation.x, { 1, 0, 0 } } * physx::PxQuat{ rotation.y, { 0, 1, 0 } } * physx::PxQuat{ rotation.z, { 0, 0, 1 } });
		m_pRigidActor->setGlobalPose(pxTransform);
	}
}