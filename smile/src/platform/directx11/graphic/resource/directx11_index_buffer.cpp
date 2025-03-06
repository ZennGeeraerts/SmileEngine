/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_index_buffer.h"

namespace smile::graphic
{
    DirectX11IndexBuffer::~DirectX11IndexBuffer()
    {
        SAFE_RELEASE( pInternal );
    }
}