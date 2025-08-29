/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/format.h"

#include <dxgi.h>

namespace smile::graphic
{
    struct DXGIFormatMapping final
    {
        Format AbstractFormat;
        DXGI_FORMAT ResourceFormat;
        DXGI_FORMAT SRVFormat;
        DXGI_FORMAT RTVFormat;
    };

    const DXGIFormatMapping &GetDXGIFormatMapping( Format abstractFormat );
}