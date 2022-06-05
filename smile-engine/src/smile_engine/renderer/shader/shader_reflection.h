#pragma once

#include "shader.h"

namespace smile
{
    struct ShaderVariable final
    {
        std::string m_Semantic{};
        ShaderDataType m_Type{};
    };

    namespace utils
    {
        std::vector< ShaderVariable > ReflectShaderVariables( const Ref< Shader > &pShader );
    }
}