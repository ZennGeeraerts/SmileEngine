/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic::rhi
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

    struct SamplerDescriptor final
    {
        SamplerFiltering Filtering;
        SamplerAddressing AddressingU;
        SamplerAddressing AddressingV;
        SamplerAddressing AddressingW;

        [[nodiscard]] foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< Uint32 >{}( static_cast< Uint32 >( Filtering ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( static_cast< Uint32 >( AddressingU ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( static_cast< Uint32 >( AddressingV ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( static_cast< Uint32 >( AddressingW ) ) );

            return hash;
        }

        bool operator==( const SamplerDescriptor &other ) const noexcept
        {
            return Filtering == other.Filtering && AddressingU == other.AddressingU &&
                   AddressingV == other.AddressingV && AddressingW == other.AddressingW;
        }

        bool operator!=( const SamplerDescriptor &other ) const noexcept
        {
            return !( *this == other );
        }
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::SamplerDescriptor >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::SamplerDescriptor &desc ) const noexcept
        {
            return desc.GetHashCode();
        }
    };
}