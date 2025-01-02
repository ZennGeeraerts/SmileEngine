/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/engine/common/compiled/pimpl.h"

namespace smile::physics
{
    class PhysicsMaterial final
    {
      private:
        struct Opaque;

      public:
        PhysicsMaterial( float staticFriction = 0.3f, float dynamicFriction = 0.3f, float restitution = 0.3f );
        ~PhysicsMaterial();

        void *GetInternal() const;

      private:
        compiled::PImpl< Opaque > m_pImplementation;
    };
}