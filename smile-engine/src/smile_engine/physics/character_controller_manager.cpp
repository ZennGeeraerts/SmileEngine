/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "character_controller_manager.h"

namespace smile::physics
{
    CharacterControllerManager::CharacterControllerManager( PhysicsWorld *pPhysicsWorld )
        : m_pPhysicsWorld{ pPhysicsWorld }
    {
    }
}