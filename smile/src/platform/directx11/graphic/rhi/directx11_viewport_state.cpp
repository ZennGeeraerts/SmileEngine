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
 * @file        directx11_viewport_state.cpp
 * @author      Zenn Geeraerts
 * @created     7 Januari 2026
 * @brief       DirectX 11 implementation of a viewport state
 */
#include "smpch.h"
#include "directx11_viewport_state.h"

namespace smile::graphic::rhi
{
    DirectX11ViewportState::DirectX11ViewportState( const ViewportState &viewport )
        : ViewportCount{ viewport.Viewports.GetItemCount() }, ScissorRectCount{ viewport.ScissorRects.GetItemCount() }
    {
        for ( Index i{}; i < viewport.Viewports.GetItemCount(); ++i )
        {
            Viewports[i].TopLeftX = viewport.Viewports[i].MinX;
            Viewports[i].TopLeftY = viewport.Viewports[i].MinY;
            Viewports[i].Width = viewport.Viewports[i].MaxX - viewport.Viewports[i].MinX;
            Viewports[i].Height = viewport.Viewports[i].MaxY - viewport.Viewports[i].MinY;
            Viewports[i].MinDepth = viewport.Viewports[i].MinZ;
            Viewports[i].MaxDepth = viewport.Viewports[i].MaxZ;
        }

        for ( Index i{}; i < viewport.ScissorRects.GetItemCount(); ++i )
        {
            ScissorRects[i].left = static_cast< LONG >( viewport.ScissorRects[i].x );
            ScissorRects[i].top = static_cast< LONG >( viewport.ScissorRects[i].y );
            ScissorRects[i].right = static_cast< LONG >( viewport.ScissorRects[i].x + viewport.ScissorRects[i].Width );
            ScissorRects[i].bottom =
                static_cast< LONG >( viewport.ScissorRects[i].y + viewport.ScissorRects[i].Height );
        }
    }
}