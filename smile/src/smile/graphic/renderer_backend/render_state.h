/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    enum class PrimitiveTopology
    {
        None = 0,
        TriangleList,
        LineList
    };

    enum class CullMode
    {
        None,
        Front,
        Back
    };

    enum class FillMode
    {
        WireFrame,
        Solid
    };

    enum class DepthWriteMask
    {
        Zero,
        All
    };

    enum class DepthComparissonFunc
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
    };
}