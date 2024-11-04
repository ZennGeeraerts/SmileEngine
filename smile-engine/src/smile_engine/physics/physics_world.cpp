/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "physics_world.h"

namespace smile::physics
{
    PhysicsWorld::PhysicsWorld( const PhysicsEngine *pPhysicsEngine, const PhysicsWorldSettings &settings )
        : m_pPhysicsEngine{ pPhysicsEngine }, m_Settings{ settings }
    {
    }
}