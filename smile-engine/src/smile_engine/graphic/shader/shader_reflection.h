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
        std::vector< ShaderVariable > ReflectShaderVariables( const Ref< Shader > &pShader );
    }
}