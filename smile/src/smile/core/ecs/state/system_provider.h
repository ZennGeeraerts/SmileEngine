/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "memory/ref.h"

namespace smile::ecs::state
{
    class SystemProvider
    {
      public:
        SystemProvider() = default;
        virtual ~SystemProvider() = default;

        virtual memory::Ref< smile::ecs::BaseSystem > Create() = 0;
    };
}