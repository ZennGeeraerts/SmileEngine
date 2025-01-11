/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "entity_handle.h"
#include "engine/common/primitive/collection/sparse_set.h"
#include "engine/common/compiled/compiled.h"

namespace smile::ecs
{
    using IndexType = typename Uint32;
    using EntityHandleType = typename EntityHandle< IndexType >;
    using SparseSetType = typename primitive::SparseSet< IndexType >;
}