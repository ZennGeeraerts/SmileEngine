/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"

namespace smile::graphic
{
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

        bool operator()( const RasterizerState &lhs, const RasterizerState &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
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

        bool operator()( const DepthStencilState &lhs, const DepthStencilState &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
        }
    };

    struct RenderState final
    {
        RasterizerState RasterizerState;
        DepthStencilState DepthStencilState;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, RasterizerState.GetHashCode() );
            hash = foundation::HashCombine( hash, DepthStencilState.GetHashCode() );
            return hash;
        }

        bool operator()( const RenderState &lhs, const RenderState &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
        }
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::RasterizerState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::RasterizerState &rasterizerState ) const
        {
            return rasterizerState.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::DepthStencilState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::DepthStencilState &depthStencilState ) const
        {
            return depthStencilState.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::RenderState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::RenderState &renderState ) const
        {
            return renderState.GetHashCode();
        }
    };
}