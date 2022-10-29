#pragma once

#include "shader.h"

namespace Smile::Graphic
{
    struct ShaderVariable final
    {
        std::string Semantic{};
        ShaderDataType Type{};
    };

    namespace Utils
    {
        std::vector< ShaderVariable > ReflectShaderVariables( const Ref< Shader > &pShader );
    }
}