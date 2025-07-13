/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic::rhi
{
    enum class CPUAccessMode : Uint8
    {
        None,
        Read,
        Write
    };
}