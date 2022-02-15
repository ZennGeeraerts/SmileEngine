#include "smpch.h"
#include "PhysicsEngine.h"

#include "SmileEngine/Core/Logger.h"
#include "PhysicsUtils.h"
#include "PhysicsDiagnostics.h"
#include "ContactListener.h"

#include <PxPhysicsAPI.h>

namespace Smile
{
    std::unordered_map<UUID, Ref<PhysicsActor>> PhysicsEngine::m_ActorMap{};
    PhysicsSettings PhysicsEngine::m_Settings{};
    PhysicsEngineData PhysicsEngine::m_PhysicsEngineData{};

    static physx::PxScene* m_pScene;

    static physx::PxDefaultAllocator m_AllocatorCallback;
    static physx::PxDefaultCpuDispatcher* m_pDefaultCpuDispatcher{};
    static physx::PxFoundation* m_pFoundation{};
    static physx::PxPvd* m_pPVD{};
    static physx::PxPhysics* m_pPhysics{};
    static physx::PxCooking* m_pCookingFactory{};

    static PhysicsErrorCallback m_ErrorCallback;
    static PhysicsAssertHandler m_AssertHandler{};
    static ContactListener m_ContactListener{};

	void PhysicsEngine::Initialize()
	{
        SM_ASSERT(!m_pFoundation, "PhysicsEngine::Init > Physics Engine is already initialized");

        // Setup the foundation
        m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback, m_ErrorCallback);
        SM_ASSERT(m_pFoundation, "PhysicsEngine::Init > Failed to create PhysX foundation");

        // Create a PDV instance
        m_pPVD = PxCreatePvd(*m_pFoundation);
        if (m_pPVD)
        {
            physx::PxPvdTransport* pTransport = physx::PxDefaultPvdSocketTransportCreate("localhost", 0001, 10);
            m_pPVD->connect(*pTransport, physx::PxPvdInstrumentationFlag::eALL);
        }

        // Create an instance of the PhysX physics SDK
        physx::PxTolerancesScale scale = physx::PxTolerancesScale();
        scale.length = 10;
        m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, scale, true, m_pPVD);
        SM_ASSERT(m_pPhysics, "PhysicsEngine::Init > Failed to create PhysX Physics");

        // Create the cooking factory
        m_pCookingFactory = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, m_pPhysics->getTolerancesScale());
        SM_ASSERT(m_pCookingFactory, "PhysicsEngine::Init > Failed to create PhysX Cooking");
        m_pDefaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
        PxSetAssertHandler(m_AssertHandler);

        // Create default physics material
        m_Settings.pDefaultPhysicsMaterial = CreateRef<PhysicsMaterial>();
        m_Settings.pDefaultPhysicsMaterial->StaticFriction = 0.3f;
        m_Settings.pDefaultPhysicsMaterial->DynamicFriction = 0.3f;
        m_Settings.pDefaultPhysicsMaterial->Restitution = 0.4f;

        SM_LOG_INFO("Initialized Physics Engine");
	}

    void PhysicsEngine::ShutDown()
    {
        if (m_pCookingFactory)
        {
            m_pCookingFactory->release();
            m_pCookingFactory = nullptr;
        }
        if (m_pPhysics)
        {
            m_pPhysics->release();
            m_pPhysics = nullptr;
        }
        if (m_pDefaultCpuDispatcher)
        {
            m_pDefaultCpuDispatcher->release();
            m_pDefaultCpuDispatcher = nullptr;
        }
        if (m_pFoundation)
        {
            m_pFoundation->release();
            m_pFoundation = nullptr;
        }
    }

    static physx::PxBroadPhaseType::Enum SmileToPhysXBroadPhaseType(BroadPhaseType type)
    {
        switch (type)
        {
        case BroadPhaseType::eSweepAndPrune:        return physx::PxBroadPhaseType::eSAP;
        case BroadPhaseType::eMultiBoxPrune:        return physx::PxBroadPhaseType::eMBP;
        case BroadPhaseType::eAutomaticBoxPrune:    return physx::PxBroadPhaseType::eABP;
        default:                                    return physx::PxBroadPhaseType::eABP;
        }
    }

    static physx::PxFrictionType::Enum SmileToPhysXFrictionType(FrictionType type)
    {
        switch (type)
        {
        case FrictionType::ePatch:          return physx::PxFrictionType::ePATCH;
        case FrictionType::eOneDirectional: return physx::PxFrictionType::eONE_DIRECTIONAL;
        case FrictionType::eTwoDirectional: return physx::PxFrictionType::eTWO_DIRECTIONAL;
        default:                            return physx::PxFrictionType::ePATCH;
        }
    }

	void PhysicsEngine::CreateScene()
	{
		SM_ASSERT(!m_pScene, "PhysicsEngine::CreateScene > Scene already has a physics scene");

        physx::PxSceneDesc sceneDesc{ m_pPhysics->getTolerancesScale() };
        sceneDesc.gravity = PhysicsUtils::ConvertToPhysXVector(m_Settings.Gravity);
        sceneDesc.filterShader = PhysicsUtils::SmileSimulationFilterShader;
        sceneDesc.cpuDispatcher = m_pDefaultCpuDispatcher;
        sceneDesc.simulationEventCallback = &m_ContactListener;
        sceneDesc.broadPhaseType = SmileToPhysXBroadPhaseType(m_Settings.BroadPhaseAlgorithm);
        sceneDesc.frictionType = SmileToPhysXFrictionType(m_Settings.FrictionModel);
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // Enable continuous collision detection

        SM_ASSERT(sceneDesc.isValid(), "PhysicsEngine::CreateScene > Scene descriptor is not valid");
        m_pScene = m_pPhysics->createScene(sceneDesc);

        if (m_Settings.BroadPhaseAlgorithm != BroadPhaseType::eAutomaticBoxPrune)
        {
            physx::PxBounds3* pRegionBounds = nullptr;
            physx::PxBounds3 globalBounds{ PhysicsUtils::ConvertToPhysXVector(m_Settings.WorldBoundsMin), PhysicsUtils::ConvertToPhysXVector(m_Settings.WorldBoundsMax) };
            uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(pRegionBounds, globalBounds, m_Settings.WorldBoundsSubdivisions);

            for (uint32_t i{}; i < regionCount; ++i)
            {
                physx::PxBroadPhaseRegion region{};
                region.bounds = pRegionBounds[i];
                m_pScene->addBroadPhaseRegion(region);
            }
        }
	}

	void PhysicsEngine::DestroyScene()
	{
        SM_ASSERT(m_pScene, "PhysicsEngine::DestroyScene > Scene is not valid");

        for (auto& actor : m_ActorMap)
            actor.second.reset();

        m_ActorMap.clear();

        m_pScene->release();
        m_pScene = nullptr;
	}

    Ref<PhysicsActor> PhysicsEngine::CreateActor(Entity entity)
    {
        SM_ASSERT(m_pScene, "PhysicsEngine::CreateActor > Scene is not valid");

        Ref<PhysicsActor> pActor = CreateRef<PhysicsActor>(entity);
        m_ActorMap[entity.GetUUID()] = pActor;
        m_pScene->addActor(*pActor->m_pRigidActor);
        return pActor;
    }

    Ref<PhysicsActor> PhysicsEngine::GetActorOfEntity(Entity entity)
    {
        auto it = m_ActorMap.find(entity.GetUUID());
        if (it != m_ActorMap.end())
            return (*it).second;

        return nullptr;
    }

    void PhysicsEngine::Simulate(Timestep deltaTime)
    {
        if (Advance(deltaTime))
        {
            for (const auto& actor : m_ActorMap)
                actor.second->UpdateTransform();
        }
    }

    bool PhysicsEngine::Advance(Timestep deltaTime)
    {
        SubstepStrategy(deltaTime);

        if (m_PhysicsEngineData.SubstepCount == 0)
            return false;

        for (uint32_t i{}; i < m_PhysicsEngineData.SubstepCount; ++i)
        {
            m_pScene->simulate(m_Settings.FixedTimestep);
            m_pScene->fetchResults(true);
        }

        return true;
    }

    void PhysicsEngine::SubstepStrategy(Timestep deltaTime)
    {
        if (m_PhysicsEngineData.Accumulator > m_Settings.FixedTimestep)
            m_PhysicsEngineData.Accumulator = 0.0f;

        m_PhysicsEngineData.Accumulator += deltaTime;
        if (m_PhysicsEngineData.Accumulator < m_Settings.FixedTimestep)
        {
            m_PhysicsEngineData.SubstepCount = 0;
            return;
        }

        m_PhysicsEngineData.SubstepCount = std::min(static_cast<uint32_t>(m_PhysicsEngineData.Accumulator / m_Settings.FixedTimestep), m_Settings.MaxSubsteps);
        m_PhysicsEngineData.Accumulator -= m_PhysicsEngineData.SubstepCount * m_Settings.FixedTimestep;
    }

    physx::PxPhysics* PhysicsEngine::GetPhysics()
    {
        return m_pPhysics;
    }

    physx::PxAllocatorCallback& PhysicsEngine::GetAllocatorCallback()
    {
        return m_AllocatorCallback;
    }
}