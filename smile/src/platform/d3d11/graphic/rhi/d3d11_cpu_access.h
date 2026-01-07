/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/cpu_access_mode.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    static UINT CPUAccessToD3D11Type( CPUAccessMode cpuAccess )
    {
        switch ( cpuAccess )
        {
            case CPUAccessMode::None:
                return 0;
            case CPUAccessMode::Read:
                return D3D11_CPU_ACCESS_READ;
            case CPUAccessMode::Write:
                return D3D11_CPU_ACCESS_WRITE;
            default:
                return 0;
        }
    }
}