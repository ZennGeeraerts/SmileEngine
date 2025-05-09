/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"

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

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, static_cast< foundation::HashCode >( Filtering ) );
            hash = foundation::HashCombine( hash, static_cast< foundation::HashCode >( AddressingU ) );
            hash = foundation::HashCombine( hash, static_cast< foundation::HashCode >( AddressingV ) );
            hash = foundation::HashCombine( hash, static_cast< foundation::HashCode >( AddressingW ) );
            return hash;
        }

        bool operator()( const SamplerState &lhs, const SamplerState &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
        }
    };
}