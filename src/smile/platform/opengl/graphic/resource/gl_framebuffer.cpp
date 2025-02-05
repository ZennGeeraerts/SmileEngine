/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "gl_framebuffer.h"

#include <glad/glad.h>

namespace smile::graphic
{
    GLFramebuffer::~GLFramebuffer()
    {
        glDeleteFramebuffers( 1, &Internal );
        glDeleteTextures( ColorAttachments.size(), ColorAttachments.data() );
        glDeleteTextures( 1, &DepthAttachment );
    }

    void *GLFramebuffer::GetColor( Uint32 index ) const
    {
        SM_ASSERT( index < ColorAttachments.size(), "GLFramebuffer::GetColor > Index out of range" );
        return reinterpret_cast< void * >( ColorAttachments[index] );
    }
}