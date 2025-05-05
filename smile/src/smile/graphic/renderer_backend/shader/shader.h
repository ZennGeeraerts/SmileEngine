/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::graphic
{
    enum class ShaderType
    {
        Unknown,
        Vertex,
        Pixel
    };

    struct ShaderDescriptor final
    {
        ShaderDescriptor( ShaderType type ) : Type{ type }
        {
        }

        ShaderType Type = ShaderType::Unknown;
        std::string EntryPoint = "main";
        std::string TargetProfile;
    };
}
