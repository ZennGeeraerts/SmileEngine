/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/common/foundation/constant_text.h"

namespace smile::ecs
{
    class ECSEngine;

    class BaseSystem : public memory::Counted
    {
      public:
        BaseSystem() = default;
        virtual ~BaseSystem() = default;

        virtual void OnAdd( ECSEngine &ecsEngine ) = 0;
        virtual void OnRemove( ECSEngine &ecsEngine ) = 0;
        virtual void OnUpdate() = 0;
        virtual foundation::ConstantText GetName() const = 0;
    };
}