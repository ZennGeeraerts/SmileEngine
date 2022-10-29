#pragma once

#include "entity_handle.h"
#include "smile_engine/stl/sparse_set.h"
#include "smile_engine/core/core.h"

namespace Smile::ECS
{
    using IndexType = typename Uint32;
    using EntityHandleType = typename EntityHandle< IndexType >;
    using SparseSetType = typename STL::SparseSet< IndexType >;
}