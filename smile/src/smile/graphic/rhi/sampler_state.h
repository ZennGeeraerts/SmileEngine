/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    enum class SamplerFiltering
    {
        MinMagMipPoint,
        MinMagMipLinear
    };

    enum class SamplerAddressing
    {
        Clamp,
        Wrap
    };

    struct SamplerState final
    {
        SamplerFiltering Filtering;
        SamplerAddressing AddressingU;
        SamplerAddressing AddressingV;
        SamplerAddressing AddressingW;
    };
}