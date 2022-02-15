#pragma once

#include "PhysicsActor.h"

namespace physx
{
	class PxPhysics;
	class PxAllocatorCallback;
}

namespace Smile
{
	struct PhysicsEngineData final
	{
		float Accumulator = 0.0f;
		uint32_t SubstepCount = 0;
	};

	enum class FrictionType
	{
		ePatch,
		eOneDirectional,
		eTwoDirectional
	};

	enum class BroadPhaseType
	{
		eSweepAndPrune,
		eMultiBoxPrune,
		eAutomaticBoxPrune
	};

	struct PhysicsSettings final
	{
		float FixedTimestep = 0.01f;
		uint32_t MaxSubsteps = 8;
		DirectX::XMFLOAT3 Gravity = { 0, -9.81f, 0 };

		BroadPhaseType BroadPhaseAlgorithm = BroadPhaseType::eAutomaticBoxPrune;
		FrictionType FrictionModel = FrictionType::ePatch;
		DirectX::XMFLOAT3 WorldBoundsMin = { 0, 0, 0 };
		DirectX::XMFLOAT3 WorldBoundsMax = { 1, 1, 1 };
		uint32_t WorldBoundsSubdivisions = 2;
		uint32_t SolverIterations = 6;
		uint32_t SolverVelocityIterations = 1;
		Ref<PhysicsMaterial> pDefaultPhysicsMaterial;
	};

	class PhysicsEngine final
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void CreateScene();
		static void DestroyScene();

		static Ref<PhysicsActor> CreateActor(Entity entity);
		static Ref<PhysicsActor> GetActorOfEntity(Entity entity);

		static void Simulate(Timestep deltaTime);

		static physx::PxPhysics* GetPhysics();
		static physx::PxAllocatorCallback& GetAllocatorCallback();
		static const PhysicsSettings& GetPhysicsSettings() { return m_Settings; }
		static const Ref<PhysicsMaterial>& GetDefaultPhysicsMaterial() { return m_Settings.pDefaultPhysicsMaterial; }

	private:
		static bool Advance(Timestep deltaTime);
		static void SubstepStrategy(Timestep deltaTime);

	private:
		static std::unordered_map<UUID, Ref<PhysicsActor>> m_ActorMap;
		static PhysicsSettings m_Settings;
		static PhysicsEngineData m_PhysicsEngineData;
	};
}