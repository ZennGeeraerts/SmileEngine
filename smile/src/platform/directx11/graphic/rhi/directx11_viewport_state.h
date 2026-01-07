/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        directx11_viewport_state.h
 * @author      Zenn Geeraerts
 * @created     7 Januari 2026
 * @brief       DirectX 11 implementation of a viewport state
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/array.h"
#include "smile/graphic/rhi/viewport_state.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    struct DirectX11ViewportState final
    {
        DirectX11ViewportState( const ViewportState &viewport );

        Count ViewportCount{ 0 };
        primitive::Array< D3D11_VIEWPORT, D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX > Viewports{};
        Count ScissorRectCount{ 0 };
        primitive::Array< D3D11_RECT, D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX > ScissorRects{};
    };
}