/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_vertex_array_object.h"

#include <glad/glad.h>

namespace smile::graphic
{
    GLVertexArrayObject::~GLVertexArrayObject()
    {
        glDeleteVertexArrays( 1, &Internal );
    }

    void *GLVertexArrayObject::GetInternal() const
    {
        return reinterpret_cast< void * >( Internal );
    }
}