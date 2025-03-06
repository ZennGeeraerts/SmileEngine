/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_rasterizer_state.h"

namespace smile::graphic
{
    DirectX11RasterizerState::~DirectX11RasterizerState()
    {
        SAFE_RELEASE( pInternal );
    }
}