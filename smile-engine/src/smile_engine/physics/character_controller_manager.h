/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "character_controller.h"
#include "ecs/character_controller_component.h"

namespace smile::physics
{
    class PhysicsWorld;

    class CharacterControllerManager
    {
      public:
        CharacterControllerManager( PhysicsWorld *pPhysicsWorld );
        ~CharacterControllerManager() = default;

        virtual Ref< CharacterController > CreateController(
            const ecs::CharacterControllerComponent &characterControllerComponent ) = 0;

      protected:
        PhysicsWorld *m_pPhysicsWorld;
    };
}