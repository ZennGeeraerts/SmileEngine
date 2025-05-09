/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "directx11_state_cache.h"
#include "resource/directx11_sampler_state.h"

namespace smile::graphic
{
    using DirectX11SamplerStateCache = typename DirectX11StateCache< SamplerState, DirectX11SamplerState >;
}