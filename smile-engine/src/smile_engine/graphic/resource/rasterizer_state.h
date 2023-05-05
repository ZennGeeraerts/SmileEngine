/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
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

    struct RasterizerStateDescriptor final
    {
        CullMode CullMode = CullMode::None;
        FillMode FillMode = FillMode::Solid;
        bool EnableDepthClip = true;
    };

    struct RasterizerState
    {
        RasterizerState() = default;
        virtual ~RasterizerState() = default;

        virtual void *GetInternal() const = 0;
    };
}