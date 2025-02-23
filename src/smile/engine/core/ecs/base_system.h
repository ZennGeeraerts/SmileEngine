/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "memory/object.h"

namespace smile::ecs
{
    class ECSEngine;

    class BaseSystem : public memory::Object
    {
      public:
        BaseSystem() = default;
        virtual ~BaseSystem() = default;

        virtual void OnAdd( ECSEngine &ecsEngine ) = 0;
        virtual void OnRemove( ECSEngine &ecsEngine ) = 0;
        virtual void OnUpdate() = 0;
        virtual std::string_view GetName() const = 0;
    };
}