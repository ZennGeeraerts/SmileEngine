/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_state.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    static D3D11_PRIMITIVE_TOPOLOGY ConvertToD3D11PrimitiveTopology( PrimitiveTopology primitiveTopology )
    {
        switch ( primitiveTopology )
        {
            case PrimitiveTopology::None:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            case PrimitiveTopology::TriangleList:
                return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case PrimitiveTopology::LineList:
                return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            default:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        }
    }
}