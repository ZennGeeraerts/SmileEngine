/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "gl_device.h"

#include "graphics_context.h"

#include "resource/gl_vertex_buffer.h"
#include "resource/gl_index_buffer.h"
#include "resource/gl_vertex_array_object.h"
#include "resource/gl_framebuffer.h"
#include "shader/gl_shader.h"

#include <fstream>
#include <array>

#include <glad/glad.h>

namespace smile::graphic
{
    namespace utils
    {
        static GLenum ShaderTypeFromString( const std::string &type )
        {
            if ( type == "vertex" )
                return GL_VERTEX_SHADER;
            else if ( type == "fragment" || type == "pixel" )
                return GL_FRAGMENT_SHADER;

            // Geometry shaders are currently not supported

            SM_ASSERT( false, "Unknown shader type." );
            return 0;
        }

        static std::string ReadShaderFile( const std::filesystem::path &filePath )
        {
            std::string result{};

            std::ifstream inputFile{ filePath, std::ios::in | std::ios::binary };
            if ( inputFile.is_open() )
            {
                inputFile.seekg( 0, std::ios::end );
                size_t size = inputFile.tellg();
                if ( size != -1 )
                {
                    result.resize( size );
                    inputFile.seekg( 0, std::ios::beg );
                    inputFile.read( &result[0], size );
                }
                else
                {
                    SM_LOG_ERROR( "Failed to read file '{0}'", filePath.string() );
                }
            }
            else
            {
                SM_LOG_ERROR( "Failed to open file '{0}'", filePath.string() );
            }

            return result;
        }

        static std::unordered_map< GLenum, std::string > PreProcessShader( const std::string &source )
        {
            std::unordered_map< GLenum, std::string > shaderSources{};

            const char *typeToken = "#type";
            const size_t typeTokenLength = strlen( typeToken );
            size_t pos = source.find( typeToken, 0 );

            while ( pos != std::string::npos )
            {
                const size_t eol = source.find_first_of( "\r\n", pos );

                ASSERT( eol != std::string::npos, "Syntax error." );

                const size_t begin = pos + typeTokenLength + 1;
                const std::string type = source.substr( begin, eol - begin );

                ASSERT( ShaderTypeFromString( type ), "Invalid shader type specified." );

                size_t nextLinePos = source.find_first_not_of( "\r\n", eol );

                ASSERT( nextLinePos != std::string::npos, "Syntax error." );

                pos = source.find( typeToken, nextLinePos );

                shaderSources[ShaderTypeFromString( type )] = ( pos == std::string::npos )
                                                                  ? source.substr( nextLinePos )
                                                                  : source.substr( nextLinePos, pos - nextLinePos );
            }

            return shaderSources;
        }

        static void CompileShader( std::shared_ptr< GLShader > pShader,
            const std::unordered_map< GLenum, std::string > &shaderSources )
        {
            GLuint program = glCreateProgram();
            ASSERT( shaderSources.size() <= 2, "Only 2 shaders are supported." );

            std::array< GLenum, 2 > glShaderIDs;
            int glShaderIDIndex = 0;
            for ( auto &kv : shaderSources )
            {
                GLenum type = kv.first;
                const std::string &source = kv.second;

                GLuint shader = glCreateShader( type );

                const GLchar *sourceCStr = source.c_str();
                glShaderSource( shader, 1, &sourceCStr, 0 );

                glCompileShader( shader );

                GLint isCompiled = 0;
                glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
                if ( isCompiled == GL_FALSE )
                {
                    GLint maxLength = 0;
                    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

                    std::vector< GLchar > infoLog( maxLength );
                    glGetShaderInfoLog( shader, maxLength, &maxLength, &infoLog[0] );

                    glDeleteShader( shader );

                    LOG_ERROR( "{0}", infoLog.data() );
                    ASSERT( false, "Shader compilation failed." );
                    break;
                }

                glAttachShader( program, shader );
                glShaderIDs[glShaderIDIndex++] = shader;
            }

            pShader->Internal = program;

            glLinkProgram( program );

            GLint isLinked = 0;
            glGetProgramiv( program, GL_LINK_STATUS, static_cast< int * >( &isLinked ) );
            if ( isLinked == GL_FALSE )
            {
                GLint maxLength = 0;
                glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

                std::vector< GLchar > infoLog( maxLength );
                glGetProgramInfoLog( program, maxLength, &maxLength, &infoLog[0] );

                glDeleteProgram( program );

                for ( auto id : glShaderIDs )
                    glDeleteShader( id );

                LOG_ERROR( "{0}", infoLog.data() );
                ASSERT( false, "Shader failed to link." );
                return;
            }

            for ( auto id : glShaderIDs )
            {
                glDetachShader( program, id );
                glDeleteShader( id );
            }
        }

        static GLenum ShaderDataTypeToOpenGLBaseType( ShaderDataType type )
        {
            switch ( type )
            {
                case ShaderDataType::Float:
                    return GL_FLOAT;
                case ShaderDataType::Float2:
                    return GL_FLOAT;
                case ShaderDataType::Float3:
                    return GL_FLOAT;
                case ShaderDataType::Float4:
                    return GL_FLOAT;
                case ShaderDataType::Mat3:
                    return GL_FLOAT;
                case ShaderDataType::Mat4:
                    return GL_FLOAT;
                case ShaderDataType::Int:
                    return GL_INT;
                case ShaderDataType::Int2:
                    return GL_INT;
                case ShaderDataType::Int3:
                    return GL_INT;
                case ShaderDataType::Int4:
                    return GL_INT;
                case ShaderDataType::Bool:
                    return GL_BOOL;
            }

            ASSERT( false, "Unknown ShaderDataType." );
            return 0;
        }

        static GLenum TextureTarget( bool multisampled )
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures( bool multisampled, Uint32 *pOutId, Uint32 count )
        {
            glCreateTextures( TextureTarget( multisampled ), count, pOutId );
        }

        static void BindTexture( bool multisampled, Uint32 id )
        {
            glBindTexture( TextureTarget( multisampled ), id );
        }

        static void AttachColorTexture( Uint32 id,
            int samples,
            GLenum internalFormat,
            GLenum format,
            Uint32 width,
            Uint32 height,
            int index )
        {
            bool multisampled = samples > 1;
            if ( multisampled )
            {
                glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE );
            }
            else
            {
                glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            }

            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget( multisampled ), id, 0 );
        }

        static void
        AttachDepthTexture( Uint32 id, int samples, GLenum format, GLenum attachmentType, Uint32 width, Uint32 height )
        {
            bool multisampled = samples > 1;
            if ( multisampled )
            {
                glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE );
            }
            else
            {
                glTexStorage2D( GL_TEXTURE_2D, 1, format, width, height );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            }

            glFramebufferTexture2D( GL_FRAMEBUFFER, attachmentType, TextureTarget( multisampled ), id, 0 );
        }

        static void InvalidateFramebuffer( std::shared_ptr< GLFramebuffer > pFramebuffer )
        {
            if ( pFramebuffer->Internal )
            {
                glDeleteFramebuffers( 1, &pFramebuffer->Internal );
                glDeleteTextures( pFramebuffer->ColorAttachments.size(), pFramebuffer->ColorAttachments.data() );
                glDeleteTextures( 1, &pFramebuffer->DepthAttachment );

                pFramebuffer->ColorAttachments.clear();
                pFramebuffer->DepthAttachment = 0;
            }

            glCreateFramebuffers( 1, &pFramebuffer->Internal );
            glBindFramebuffer( GL_FRAMEBUFFER, pFramebuffer->Internal );

            bool multisample = pFramebuffer->Descriptor.Samples > 1;

            if ( pFramebuffer->ColorAttachmentData.size() )
            {
                pFramebuffer->ColorAttachments.resize( pFramebuffer->ColorAttachmentData.size() );
                CreateTextures(
                    multisample, pFramebuffer->ColorAttachments.data(), pFramebuffer->ColorAttachments.size() );

                for ( size_t i{}; i < pFramebuffer->ColorAttachments.size(); ++i )
                {
                    BindTexture( multisample, pFramebuffer->ColorAttachments[i] );
                    switch ( pFramebuffer->ColorAttachmentData[i].TextureFormat )
                    {
                        case FramebufferTextureFormat::RGBA8:
                            AttachColorTexture( pFramebuffer->ColorAttachments[i],
                                pFramebuffer->Descriptor.Samples,
                                GL_RGBA8,
                                GL_RGBA,
                                pFramebuffer->Descriptor.Width,
                                pFramebuffer->Descriptor.Height,
                                i );
                            break;
                        case FramebufferTextureFormat::RedInteger:
                            AttachColorTexture( pFramebuffer->ColorAttachments[i],
                                pFramebuffer->Descriptor.Samples,
                                GL_R32I,
                                GL_RED_INTEGER,
                                pFramebuffer->Descriptor.Width,
                                pFramebuffer->Descriptor.Height,
                                i );
                            break;
                    }
                }
            }

            if ( pFramebuffer->DepthAttachmentData.TextureFormat != FramebufferTextureFormat::None )
            {
                CreateTextures( multisample, &pFramebuffer->DepthAttachment, 1 );
                BindTexture( multisample, pFramebuffer->DepthAttachment );
                switch ( pFramebuffer->DepthAttachmentData.TextureFormat )
                {
                    case FramebufferTextureFormat::Depth24Stencil8:
                        AttachDepthTexture( pFramebuffer->DepthAttachment,
                            pFramebuffer->Descriptor.Samples,
                            GL_DEPTH24_STENCIL8,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            pFramebuffer->Descriptor.Width,
                            pFramebuffer->Descriptor.Height );
                        break;
                }
            }

            if ( pFramebuffer->ColorAttachments.size() > 1 )
            {
                ASSERT( pFramebuffer->ColorAttachments.size() <= 4, "Maximum framebuffer size reached." );
                GLenum buffers[4]{
                    GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
                glDrawBuffers( pFramebuffer->ColorAttachments.size(), buffers );
            }
            else
            {
                glDrawBuffer( GL_NONE );
            }

            ASSERT(
                glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete." );

            glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        }
    }

    void GLDevice::Initialize( GraphicsContext *pContext )
    {
        m_pContext = pContext;
    }

    std::shared_ptr< VertexBuffer > GLDevice::CreateVertexBuffer( const VertexBufferDescriptor &desc ) const
    {
        auto pVertexBuffer = std::make_shared< GLVertexBuffer >();
        pVertexBuffer->Layout = desc.Layout;

        glCreateBuffers( 1, &pVertexBuffer->Internal );
        glBindBuffer( GL_ARRAY_BUFFER, pVertexBuffer->Internal );
        glBufferData( GL_ARRAY_BUFFER, desc.Count * desc.Layout.GetStride(), desc.pVertices, GL_STATIC_DRAW );

        return pVertexBuffer;
    }

    std::shared_ptr< IndexBuffer > GLDevice::CreateIndexBuffer( const IndexBufferDescriptor &desc ) const
    {
        auto pIndexBuffer = std::make_shared< GLIndexBuffer >();
        pIndexBuffer->Count = desc.Count;

        glCreateBuffers( 1, &pIndexBuffer->Internal );
        glBindBuffer( GL_ARRAY_BUFFER, pIndexBuffer->Internal );
        glBufferData( GL_ARRAY_BUFFER, desc.Count * sizeof( Uint32 ), desc.pIndices, GL_STATIC_DRAW );

        return pIndexBuffer;
    }

    std::shared_ptr< Shader > GLDevice::CreateShader( const std::string &name,
        const std::string &vertexShaderSrc,
        const std::string &fragmentShaderSrc ) const
    {
        auto pShader = std::make_shared< GLShader >();
        pShader->Name = name;

        std::unordered_map< GLenum, std::string > sources;
        sources[GL_VERTEX_SHADER] = vertexShaderSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentShaderSrc;
        utils::CompileShader( pShader, sources );

        return pShader;
    }

    std::shared_ptr< Shader > GLDevice::CreateShader( const std::filesystem::path &filePath ) const
    {
        auto pShader = std::make_shared< GLShader >();
        pShader->Name = filePath.filename().replace_extension().string();

        std::string source = utils::ReadShaderFile( filePath );
        auto shaderSources = utils::PreProcessShader( source );
        utils::CompileShader( pShader, shaderSources );

        return pShader;
    }

    std::shared_ptr< VertexArrayObject > GLDevice::CreateVertexArrayObject(
        const std::vector< std::shared_ptr< VertexBuffer > > &pVertexBuffers,
        std::shared_ptr< IndexBuffer > pIndexBuffer ) const
    {
        auto pVertexArrayObject = std::make_shared< GLVertexArrayObject >();
        glCreateVertexArrays( 1, &pVertexArrayObject->Internal );

        glBindVertexArray( pVertexArrayObject->Internal );

        for ( auto pVertexBuffer : pVertexBuffers )
        {
            ASSERT( pVertexBuffer->Layout.GetElements().size(), "Vertex buffer has no layout." );

            m_pContext->BindVertexBuffer( pVertexBuffer );

            const auto &layout = pVertexBuffer->Layout;
            for ( const auto &element : layout )
            {
                switch ( element.DataType )
                {
                    case ShaderDataType::Float:
                    case ShaderDataType::Float2:
                    case ShaderDataType::Float3:
                    case ShaderDataType::Float4:
                    {
                        glEnableVertexAttribArray( pVertexArrayObject->VertexBufferIndex );
                        glVertexAttribPointer( pVertexArrayObject->VertexBufferIndex,
                            element.GetComponentCount(),
                            utils::ShaderDataTypeToOpenGLBaseType( element.DataType ),
                            element.IsNormalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            reinterpret_cast< const void * >( element.Offset ) );
                        pVertexArrayObject->VertexBufferIndex++;
                        break;
                    }
                    case ShaderDataType::Int:
                    case ShaderDataType::Int2:
                    case ShaderDataType::Int3:
                    case ShaderDataType::Int4:
                    case ShaderDataType::Bool:
                    {
                        glEnableVertexAttribArray( pVertexArrayObject->VertexBufferIndex );
                        glVertexAttribIPointer( pVertexArrayObject->VertexBufferIndex,
                            element.GetComponentCount(),
                            utils::ShaderDataTypeToOpenGLBaseType( element.DataType ),
                            layout.GetStride(),
                            reinterpret_cast< const void * >( element.Offset ) );
                        pVertexArrayObject->VertexBufferIndex++;
                        break;
                    }
                    case ShaderDataType::Mat3:
                    case ShaderDataType::Mat4:
                    {
                        uint8_t count = element.GetComponentCount();
                        for ( uint8_t i = 0; i < count; i++ )
                        {
                            glEnableVertexAttribArray( pVertexArrayObject->VertexBufferIndex );
                            glVertexAttribPointer( pVertexArrayObject->VertexBufferIndex,
                                count,
                                utils::ShaderDataTypeToOpenGLBaseType( element.DataType ),
                                element.IsNormalized ? GL_TRUE : GL_FALSE,
                                layout.GetStride(),
                                reinterpret_cast< const void * >( element.Offset + sizeof( float ) * count * i ) );
                            glVertexAttribDivisor( pVertexArrayObject->VertexBufferIndex, 1 );
                            pVertexArrayObject->VertexBufferIndex++;
                        }
                        break;
                    }
                    default:
                        ASSERT( false, "Unknown ShaderDataType." );
                }
            }

            pVertexArrayObject->pVertexBuffers.push_back( pVertexBuffer );
        }

        if ( pIndexBuffer )
        {
            m_pContext->BindIndexBuffer( pIndexBuffer );
            pVertexArrayObject->pIndexBuffer = pIndexBuffer;
        }

        return pVertexArrayObject;
    }

    std::shared_ptr< Framebuffer > GLDevice::CreateFramebuffer( const FramebufferDescriptor &desc ) const
    {
        auto pFramebuffer = std::make_shared< GLFramebuffer >();
        pFramebuffer->Descriptor = desc;

        for ( auto attachment : desc.Attachments.Attachments )
        {
            if ( !Framebuffer::IsDepthFormat( attachment.TextureFormat ) )
                pFramebuffer->ColorAttachmentData.emplace_back( attachment );
            else
                pFramebuffer->DepthAttachmentData = attachment;
        }

        utils::InvalidateFramebuffer( pFramebuffer );

        return pFramebuffer;
    }

    void GLDevice::ResizeFramebuffer( std::shared_ptr< Framebuffer > pFramebuffer, Uint32 width, Uint32 height ) const
    {
        if ( width == 0 || height == 0 )
        {
            SM_LOG_WARNING( "Attempted to rezize framebuffer to {0}, {1}.", width, height );
            return;
        }

        pFramebuffer->Descriptor.Width = width;
        pFramebuffer->Descriptor.Height = height;

        auto pGLFramebuffer = std::static_pointer_cast< GLFramebuffer >( pFramebuffer );
        utils::InvalidateFramebuffer( pGLFramebuffer );
    }
}