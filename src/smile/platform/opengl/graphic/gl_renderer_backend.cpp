/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_renderer_backend.h"

#include <glad/glad.h>

namespace smile::graphic
{
    void GLAPIENTRY OpenGLMessageCallback( unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char *message,
        const void *userParam )
    {
        switch ( severity )
        {
            case GL_DEBUG_SEVERITY_HIGH:
                SM_LOG_CRITICALERROR( message );
                return;
            case GL_DEBUG_SEVERITY_MEDIUM:
                SM_LOG_ERROR( message );
                return;
            case GL_DEBUG_SEVERITY_LOW:
                SM_LOG_WARNING( message );
                return;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                SM_LOG_TRACE( message );
                return;
        }

        ASSERT( false, "Unknown severity level!" );
    }

    GLRendererBackend::GLRendererBackend() : RendererBackend( Backend::OpenGL )
    {
    }

    void GLRendererBackend::Initialize( const common::PlatformData &platformData )
    {
        int status = gladLoadGLLoader( reinterpret_cast< GLADloadproc >( platformData.GLADLoadProc ) );
        SM_ASSERT( status, "Failed to initialize Glad." );

        SM_LOG_INFO( "OpenGL info:" );
        SM_LOG_INFO( "	Vendor: {0}", reinterpret_cast< const char * >( glGetString( GL_VENDOR ) ) );
        SM_LOG_INFO( "	Renderer: {0}", reinterpret_cast< const char * >( glGetString( GL_RENDERER ) ) );
        SM_LOG_INFO( "	Version: {0}", reinterpret_cast< const char * >( glGetString( GL_VERSION ) ) );

        SM_ASSERT( GLVersion.major > 4 || ( GLVersion.major == 4 && GLVersion.minor >= 5 ),
            "OpenGL version 4.5 or higher is required." );

#ifdef SM_C_DEBUG
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
        glDebugMessageCallback( OpenGLMessageCallback, nullptr );

        glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE );
#endif

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LINE_SMOOTH );
    }

    void GLRendererBackend::ResizeWindow( Uint32 x, Uint32 y, Uint32 width, Uint32 height )
    {
        glViewport( x, y, width, height );
    }

    void GLRendererBackend::SetClearColor( float r, float g, float b, float a )
    {
        glClearColor( r, g, b, a );
    }

    void GLRendererBackend::Clear()
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    void GLRendererBackend::Draw( Uint32 vertexCount )
    {
    }

    void GLRendererBackend::DrawIndexed( Uint32 indexCount )
    {
        glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr );
    }

    void GLRendererBackend::DrawLines( Uint32 vertexCount )
    {
        glDrawArrays( GL_LINES, 0, vertexCount );
    }

    void GLRendererBackend::SetLineWidth( float width )
    {
        glLineWidth( width );
    }
}