/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_vertex_buffer.h"

#include <glad/glad.h>

namespace graphicslib
{
    GLVertexBuffer::~GLVertexBuffer()
    {
        glDeleteBuffers( 1, &Internal );
    }

    void *GLVertexBuffer::GetInternal() const
    {
        return reinterpret_cast< void * >( Internal );
    }
}