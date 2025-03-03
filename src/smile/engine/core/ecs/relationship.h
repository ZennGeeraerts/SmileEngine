/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle.h"

namespace smile::ecs
{
    struct Relationship final
    {
        Uint32 ChildrenCount{};
        EntityHandle First = EntityHandle::NullHandle();
        EntityHandle Prev = EntityHandle::NullHandle();
        EntityHandle Next = EntityHandle::NullHandle();
        EntityHandle Parent = EntityHandle::NullHandle();
    };
}