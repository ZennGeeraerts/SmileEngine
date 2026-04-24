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
 * @file        d3d11_binding_layout.h
 * @author      Zenn Geeraerts
 * @created     23 April 2026
 * @brief       D3D11 binding layout
 */
#pragma once

#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic::rhi
{
    struct D3D11BindingLayout final
    {
        D3D11BindingLayout() = default;
        ~D3D11BindingLayout() = default;

        D3D11BindingLayout( const D3D11BindingLayout & ) = delete;
        D3D11BindingLayout( D3D11BindingLayout && ) = delete;
        D3D11BindingLayout &operator=( const D3D11BindingLayout & ) = delete;
        D3D11BindingLayout &operator=( D3D11BindingLayout && ) = delete;

        void Create( const BindingLayout &layout ) noexcept
        {
            Internal = layout;
        }

        BindingLayout Internal;
    };
}