/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "directx11_state_cache.h"
#include "shader/directx11_input_layout.h"

namespace smile::graphic
{
    using DirectX11InputLayoutCache = typename DirectX11StateCache< BufferLayout, DirectX11InputLayout >;
}