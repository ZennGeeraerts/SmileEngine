/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/handle_manager.h"

namespace smile::ecs
{
    using IndexType = Uint32;
    using EntityHandleManager = typename primitive::HandleManager< IndexType >;
    using EntityHandle = EntityHandleManager::HandleType;
}