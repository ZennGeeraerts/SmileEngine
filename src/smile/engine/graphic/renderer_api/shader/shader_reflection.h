/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader.h"

namespace smile::graphic
{
    struct ShaderVariable final
    {
        std::string Semantic{};
        ShaderDataType Type{};
    };

    namespace utils
    {
        std::vector< ShaderVariable > ReflectShaderVariables( const memory::Ref< Shader > &pShader );
    }
}