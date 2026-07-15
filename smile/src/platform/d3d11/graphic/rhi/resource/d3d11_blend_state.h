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
 * @file        d3d11_blend_state.h
 * @author      Zenn Geeraerts
 * @created     9 July 2026
 * @brief       D3D11 implementation of a blend state
 */
#pragma once

#include "smile/graphic/rhi/render_state.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace smile::graphic::rhi
{
    struct D3D11BlendState final
    {
        D3D11BlendState() = default;
        ~D3D11BlendState();

        D3D11BlendState( const D3D11BlendState & ) = delete;
        D3D11BlendState( D3D11BlendState && ) = delete;
        D3D11BlendState &operator=( const D3D11BlendState & ) = delete;
        D3D11BlendState &operator=( D3D11BlendState && ) = delete;

        void Create( ID3D11Device *pDevice, const BlendState &blendState );
        void Destroy();

        Microsoft::WRL::ComPtr< ID3D11BlendState > pInternal = nullptr;
    };
}