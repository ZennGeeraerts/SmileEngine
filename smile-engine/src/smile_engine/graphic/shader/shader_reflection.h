#pragma once

#include "shader.h"

namespace smile::graphic
{
    struct ShaderVariable final
    {
        std::string semantic{};
        ShaderDataType type{};
    };

    namespace utils
    {
        std::vector< ShaderVariable > reflectShaderVariables( const Ref< Shader > &shader );
    }
}