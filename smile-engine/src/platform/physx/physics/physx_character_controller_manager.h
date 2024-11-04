/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile_engine/physics/character_controller_manager.h"

namespace smile::physics
{
    class PhysXPhysicsWorld;

    class PhysXCharacterControllerManager final : public CharacterControllerManager
    {
      public:
        PhysXCharacterControllerManager( PhysXPhysicsWorld *pPhysicsWorld );

        Ref< CharacterController > CreateController(
            const ecs::CharacterControllerComponent &characterControllerComponent ) override;

      private:
        physx::PxControllerManager *m_pControllerManager;
    };
}