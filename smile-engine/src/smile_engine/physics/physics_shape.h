/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "physics_material.h"

namespace smile::physics
{
    struct PhysicsShape
    {
        PhysicsShape( const Ref< PhysicsMaterial > &pPhysicsMaterial ) : m_pPhysicsMaterial{ pPhysicsMaterial }
        {
        }
        virtual ~PhysicsShape() = default;

        Ref< PhysicsMaterial > m_pPhysicsMaterial = nullptr;
    };
}