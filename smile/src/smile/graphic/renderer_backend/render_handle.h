/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/handle_manager.h"

namespace smile::graphic
{
    using GPUBufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using GPUBufferHandle = GPUBufferHandleManager::HandleType;

    using TextureHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using TextureHandle = TextureHandleManager::HandleType;

    using FramebufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using FramebufferHandle = FramebufferHandleManager::HandleType;
}