/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directX11_vertex_buffer.h"

namespace smile::graphic
{
    DirectX11VertexBuffer::~DirectX11VertexBuffer()
    {
        SAFE_RELEASE( pInternal );
    }
}