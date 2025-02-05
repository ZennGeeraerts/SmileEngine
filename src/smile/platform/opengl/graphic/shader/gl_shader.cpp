/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_shader.h"

#include <glad/glad.h>

namespace smile::graphic
{
    GLShader::~GLShader()
    {
        glDeleteProgram( Internal );
    }

    void *GLShader::GetData() const
    {
        return reinterpret_cast< void * >( Internal );
    }
}