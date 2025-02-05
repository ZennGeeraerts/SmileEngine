/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_context.h"

#include "resource/gl_framebuffer.h"

#include <glad/glad.h>

namespace smile::graphic
{
    namespace utils
    {
        static GLenum FramebufferTextureFormatToGL( FramebufferTextureFormat format )
        {
            switch ( format )
            {
                case FramebufferTextureFormat::RGBA8:
                    return GL_RGBA8;
                case FramebufferTextureFormat::RedInteger:
                    return GL_RED_INTEGER;
            }

            ASSERT( false, "Unknown texture format." );
            return 0;
        }
    }

    void GLContext::Initialize()
    {
    }

    void GLContext::Present()
    {
    }

    void GLContext::BindVertexBuffer( std::shared_ptr< VertexBuffer > pVertexBuffer ) const
    {
        glBindBuffer( GL_ARRAY_BUFFER, reinterpret_cast< Uint32 >( pVertexBuffer->GetInternal() ) );
        ;
    }

    void GLContext::UnbindVertexBuffer() const
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void GLContext::BindIndexBuffer( std::shared_ptr< IndexBuffer > pIndexBuffer ) const
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, reinterpret_cast< Uint32 >( pIndexBuffer->GetInternal() ) );
    }

    void GLContext::UnbindIndexBuffer() const
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void GLContext::BindShader( std::shared_ptr< Shader > pShader ) const
    {
        glUseProgram( reinterpret_cast< Uint32 >( pShader->GetData() ) );
    }

    void GLContext::UnbindShader() const
    {
        glUseProgram( 0 );
    }

    void GLContext::BindVertexArrayObject( std::shared_ptr< VertexArrayObject > pVertexArrayObject ) const
    {
        glBindVertexArray( reinterpret_cast< Uint32 >( pVertexArrayObject->GetInternal() ) );
    }

    void GLContext::UnbindVertexArrayObject() const
    {
        glBindVertexArray( 0 );
    }

    void GLContext::BindFramebuffer( std::shared_ptr< Framebuffer > pFramebuffer ) const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, reinterpret_cast< Uint32 >( pFramebuffer->GetInternal() ) );
        glViewport( 0, 0, pFramebuffer->Descriptor.Width, pFramebuffer->Descriptor.Height );
    }

    void GLContext::UnbindFramebuffer() const
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void GLContext::UploadMat4( std::shared_ptr< Shader > pShader,
        const std::string &uniformName,
        const glm::mat4 &value ) const
    {
        GLint location = glGetUniformLocation( reinterpret_cast< Uint32 >( pShader->GetData() ), uniformName.c_str() );
        glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( value ) );
    }

    void GLContext::UploadVec4( std::shared_ptr< Shader > pShader,
        const std::string &uniformName,
        const glm::vec4 &value ) const
    {
        GLint location = glGetUniformLocation( reinterpret_cast< Uint32 >( pShader->GetData() ), uniformName.c_str() );
        glUniform4f( location, value.x, value.y, value.z, value.w );
    }

    void GLContext::FillVertexBuffer( std::shared_ptr< VertexBuffer > pVertexBuffer,
        const void *pData,
        Uint32 vertexCount ) const
    {
        glBindBuffer( GL_ARRAY_BUFFER, reinterpret_cast< Uint32 >( pVertexBuffer->GetInternal() ) );
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertexCount * pVertexBuffer->Layout.GetStride(), pData );
    }

    int GLContext::ReadFramebufferPixel( std::shared_ptr< Framebuffer > pFramebuffer,
        uint32_t attachmentIndex,
        int x,
        int y ) const
    {
        std::shared_ptr< GLFramebuffer > pGLFramebuffer = std::static_pointer_cast< GLFramebuffer >( pFramebuffer );

        ASSERT( attachmentIndex < pGLFramebuffer->ColorAttachments.size(), "Attachment index out of range." );

        glReadBuffer( GL_COLOR_ATTACHMENT0 + attachmentIndex );
        int pixelData{};
        glReadPixels( x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData );
        return pixelData;
    }

    std::vector< int > GLContext::ReadFramebufferPixels( std::shared_ptr< Framebuffer > pFramebuffer,
        uint32_t attachmentIndex,
        int x,
        int y,
        int width,
        int height ) const
    {
        std::shared_ptr< GLFramebuffer > pGLFramebuffer = std::static_pointer_cast< GLFramebuffer >( pFramebuffer );

        ASSERT( attachmentIndex < pGLFramebuffer->ColorAttachments.size(), "Attachment index out of range." );

        glReadBuffer( GL_COLOR_ATTACHMENT0 + attachmentIndex );
        std::vector< int > pixels{};
        pixels.resize( width * height );
        glReadPixels( x, y, width, height, GL_RED_INTEGER, GL_INT, pixels.data() );
        return pixels;
    }

    void GLContext::ClearFramebufferAttachment( std::shared_ptr< Framebuffer > pFramebuffer,
        uint32_t attachmentIndex,
        int value ) const
    {
        std::shared_ptr< GLFramebuffer > pGLFramebuffer = std::static_pointer_cast< GLFramebuffer >( pFramebuffer );

        ASSERT( attachmentIndex < pGLFramebuffer->ColorAttachments.size(), "Attachment index out of range." );

        auto &data = pGLFramebuffer->ColorAttachmentData[attachmentIndex];
        glClearTexImage( pGLFramebuffer->ColorAttachments[attachmentIndex],
            0,
            utils::FramebufferTextureFormatToGL( data.TextureFormat ),
            GL_INT,
            &value );
    }
}