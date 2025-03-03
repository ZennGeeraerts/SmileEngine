/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_backend/shader/buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage );
    UINT BufferCPUAccessToDirectXType( BufferCPUAccess cpuAccess );
}