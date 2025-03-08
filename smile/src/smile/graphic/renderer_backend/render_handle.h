/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "primitive/handle_manager.h"

namespace smile::graphic
{
    using VertexBufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using VertexBufferHandle = VertexBufferHandleManager::HandleType;

    using IndexBufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using IndexBufferHandle = IndexBufferHandleManager::HandleType;
}