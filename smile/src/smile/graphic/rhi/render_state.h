/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "resource/frame_buffer.h"

namespace smile::graphic::rhi
{
    enum class BlendFactor : Uint8
    {
        Zero = 1,
        One = 2,
        SrcColor = 3,
        InvSrcColor = 4,
        SrcAlpha = 5,
        InvSrcAlpha = 6,
        DestAlpha = 7,
        InvDestAlpha = 8,
        DestColor = 9,
        InvDestColor = 10,
        SrcAlphaSaturate = 11,
        ConstantColor = 14,
        InvConstantColor = 15,
        Src1Color = 16,
        InvSrc1Color = 17,
        Src1Alpha = 18,
        InvSrc1Alpha = 19
    };

    enum class BlendOp : Uint8
    {
        Add = 1,
        Subtract = 2,
        RevSubtract = 3,
        Min = 4,
        Max = 5
    };

    enum class ColorMask : Uint8
    {
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = Red | Green | Blue | Alpha
    };

    struct BlendState final
    {
        struct RenderTarget final
        {
            bool BlendEnabled = false;
            BlendFactor SrcBlend = BlendFactor::One;
            BlendFactor DestBlend = BlendFactor::Zero;
            BlendOp BlendOp_ = BlendOp::Add;
            BlendFactor SrcBlendAlpha = BlendFactor::One;
            BlendFactor DestBlendAlpha = BlendFactor::Zero;
            BlendOp BlendOpAlpha = BlendOp::Add;
            ColorMask ColorWriteMask = ColorMask::All;

            constexpr bool operator==( const RenderTarget &other ) const noexcept
            {
                return BlendEnabled == other.BlendEnabled && SrcBlend == other.SrcBlend &&
                       DestBlend == other.DestBlend && BlendOp_ == other.BlendOp_ &&
                       SrcBlendAlpha == other.SrcBlendAlpha && DestBlendAlpha == other.DestBlendAlpha &&
                       BlendOpAlpha == other.BlendOpAlpha && ColorWriteMask == other.ColorWriteMask;
            }

            constexpr bool operator!=( const RenderTarget &other ) const noexcept
            {
                return !( *this == other );
            }

            bool UsesConstantColor() const noexcept
            {
                return SrcBlend == BlendFactor::ConstantColor || SrcBlend == BlendFactor::InvConstantColor ||
                       DestBlend == BlendFactor::ConstantColor || DestBlend == BlendFactor::InvConstantColor ||
                       SrcBlendAlpha == BlendFactor::ConstantColor || SrcBlendAlpha == BlendFactor::InvConstantColor ||
                       DestBlendAlpha == BlendFactor::ConstantColor || DestBlendAlpha == BlendFactor::InvConstantColor;
            }
        };

        primitive::Array< RenderTarget, s_MaxRenderTargets > Targets;
        bool AlphaToCoverageEnable = false;

        constexpr bool operator==( const BlendState &other ) const noexcept
        {
            return AlphaToCoverageEnable == other.AlphaToCoverageEnable &&
                   primitive::array::IsEqual( Targets, other.Targets );
        }

        constexpr bool operator!=( const BlendState &other ) const noexcept
        {
            return !( *this == other );
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = std::hash< bool >{}( AlphaToCoverageEnable );

            for ( Index i{ 0 }; i < s_MaxRenderTargets; ++i )
            {
                hash = foundation::HashCombine( hash, std::hash< bool >{}( Targets[i].BlendEnabled ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].SrcBlend ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].DestBlend ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].BlendOp_ ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].SrcBlendAlpha ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].DestBlendAlpha ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].BlendOpAlpha ) ) );
                hash = foundation::HashCombine(
                    hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Targets[i].ColorWriteMask ) ) );
            }

            return hash;
        }

        bool UsesConstantColor( const Count targetCount ) const noexcept
        {
            for ( Index i{ 0 }; i < targetCount; ++i )
            {
                if ( Targets[i].UsesConstantColor() )
                    return true;
            }

            return false;
        }
    };

    enum class CullMode : Uint8
    {
        None,
        Front,
        Back
    };

    enum class FillMode : Uint8
    {
        WireFrame,
        Solid
    };

    struct RasterizerState final
    {
        CullMode CullMode = CullMode::Back;
        FillMode FillMode = FillMode::Solid;
        bool EnableDepthClip = true;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( CullMode ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( FillMode ) ) );
            hash = foundation::HashCombine( hash, std::hash< bool >{}( EnableDepthClip ) );
            return hash;
        }

        bool operator==( const RasterizerState &other ) const noexcept
        {
            return CullMode == other.CullMode && FillMode == other.FillMode && EnableDepthClip == other.EnableDepthClip;
        }

        bool operator!=( const RasterizerState &other ) const noexcept
        {
            return !( *this == other );
        }
    };

    enum class DepthWriteMask : Uint8
    {
        Zero,
        All
    };

    enum class DepthComparissonFunc : Uint8
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    struct DepthStencilState final
    {
        bool DepthEnable = true;
        DepthWriteMask DepthWriteMask = DepthWriteMask::All;
        DepthComparissonFunc DepthComparissonFunc = DepthComparissonFunc::Less;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< bool >{}( DepthEnable ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( DepthWriteMask ) ) );
            hash =
                foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( DepthComparissonFunc ) ) );
            return hash;
        }

        bool operator==( const DepthStencilState &other ) const noexcept
        {
            return DepthEnable == other.DepthEnable && DepthWriteMask == other.DepthWriteMask &&
                   DepthComparissonFunc == other.DepthComparissonFunc;
        }

        bool operator!=( const DepthStencilState &other ) const noexcept
        {
            return !( *this == other );
        }
    };

    struct RenderState final
    {
        BlendState BlendState;
        RasterizerState RasterizerState;
        DepthStencilState DepthStencilState;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, BlendState.GetHashCode() );
            hash = foundation::HashCombine( hash, RasterizerState.GetHashCode() );
            hash = foundation::HashCombine( hash, DepthStencilState.GetHashCode() );
            return hash;
        }

        bool operator()( const RenderState &lhs, const RenderState &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
        }

        bool operator==( const RenderState &other ) const noexcept
        {
            return BlendState == other.BlendState && RasterizerState == other.RasterizerState &&
                   DepthStencilState == other.DepthStencilState;
        }

        bool operator!=( const RenderState &other ) const noexcept
        {
            return !( *this == other );
        }
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::BlendState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::BlendState &blendState ) const
        {
            return blendState.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::rhi::RasterizerState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::RasterizerState &rasterizerState ) const
        {
            return rasterizerState.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::rhi::DepthStencilState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::DepthStencilState &depthStencilState ) const
        {
            return depthStencilState.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::rhi::RenderState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::RenderState &renderState ) const
        {
            return renderState.GetHashCode();
        }
    };
}