/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    enum class PrimitiveTopology : Uint8
    {
        None = 0,
        TriangleList,
        LineList
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

    struct RenderState final
    {
        PrimitiveTopology Topology = PrimitiveTopology::TriangleList;

        CullMode CullMode = CullMode::Back;
        FillMode FillMode = FillMode::Solid;
        bool EnableDepthClip = true;

        bool DepthEnable = true;
        DepthWriteMask DepthWriteMask = DepthWriteMask::All;
        DepthComparissonFunc DepthComparissonFunc = DepthComparissonFunc::Less;

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;

            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Topology ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( CullMode ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( FillMode ) ) );
            hash = foundation::HashCombine( hash, std::hash< bool >{}( EnableDepthClip ) );
            hash = foundation::HashCombine( hash, std::hash< bool >{}( DepthEnable ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( DepthWriteMask ) ) );
            hash =
                foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( DepthComparissonFunc ) ) );

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
    struct hash< smile::graphic::RenderState >
    {
        smile::foundation::HashCode operator()( const smile::graphic::RenderState &renderState ) const
        {
            return renderState.GetHashCode();
        }
    };
}