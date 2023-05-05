/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle.h"
#include "smile_engine/stl/sparse_set.h"
#include "smile_engine/core/core.h"

namespace smile::ecs
{
    using IndexType = typename Uint32;
    using EntityHandleType = typename EntityHandle< IndexType >;
    using SparseSetType = typename stl::SparseSet< IndexType >;
}