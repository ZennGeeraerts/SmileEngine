/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "ecs_types.h"

namespace smile::ecs
{
	struct Relationship final
	{
        Uint32 ChildrenCount{};
        EntityHandleType First = ecs::EntityHandleType::NullHandle();
        EntityHandleType Prev = ecs::EntityHandleType::NullHandle();
        EntityHandleType Next = ecs::EntityHandleType::NullHandle();
        EntityHandleType Parent = ecs::EntityHandleType::NullHandle();
	};
}