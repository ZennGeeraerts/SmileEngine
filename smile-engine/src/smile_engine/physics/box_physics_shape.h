/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "physics_shape.h"
#include "smile_engine/core/geometric/box.h"

namespace smile::physics
{
    class BoxPhysicsShape final : public PhysicsShape
    {
      public:
        BoxPhysicsShape( const Box &box, const Ref< PhysicsMaterial > &pPhysicsMaterial )
            : PhysicsShape{ pPhysicsMaterial }
        {
        }
    };
}