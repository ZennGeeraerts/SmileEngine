/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "platform/opengl/resource/gl_index_buffer.h"

#include <glad/glad.h>

namespace smile::graphic
{
    GLIndexBuffer::~GLIndexBuffer()
    {
        glDeleteBuffers( 1, &Internal );
    }

    void *GLIndexBuffer::GetInternal() const
    {
        return reinterpret_cast< void * >( Internal );
    }
}