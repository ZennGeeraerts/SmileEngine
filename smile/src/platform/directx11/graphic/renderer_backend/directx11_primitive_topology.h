/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/render_state.h"

#include <d3d11.h>

namespace smile::graphic
{
    static D3D11_PRIMITIVE_TOPOLOGY ConvertToDirectX11PrimitiveTopology( PrimitiveTopology primitiveTopology )
    {
        switch ( primitiveTopology )
        {
            case smile::graphic::PrimitiveTopology::None:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            case smile::graphic::PrimitiveTopology::TriangleList:
                return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case smile::graphic::PrimitiveTopology::LineList:
                return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            default:
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        }
    }
}