#include "smpch.h"
#include "shader.h"

#include "smile_engine/renderer/renderer.h"

#ifdef SM_PLATFORM_WINDOWS
#    include "platform/directx11/shader/directx11_shader.h"
#    include "platform/smileraster/smileraster_shader.h"
#endif

namespace smile
{
    Ref< Shader > Shader::Create( const std::string &filePath, const BufferLayout &bufferLayout )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Shader >( filePath, bufferLayout );

            case RendererAPI::API::SmileRaster:
                return CreateRef< SmileRasterShader >( filePath, bufferLayout );
#endif
        }

        SM_ASSERT( false,
            "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }

    Ref< Shader > Shader::Create( const std::string &filePath )
    {
        switch ( Renderer::GetAPI() )
        {
            case RendererAPI::API::None:
                SM_ASSERT( false, "Shader::Create > return nullptr, no renderer api selected" );
                return nullptr;

#ifdef SM_PLATFORM_WINDOWS
            case RendererAPI::API::DirectX11:
                return CreateRef< DirectX11Shader >( filePath );
#endif
        }

        SM_ASSERT( false,
            "Shader::Create > return nullptr, unknown render api or render api is not supported on this platform" );
        return nullptr;
    }

    void ShaderLibrary::Add( const std::string &name, const Ref< Shader > &pShader )
    {
        SM_ASSERT( !Exists( name ), "ShaderLibrary::Add > Shader: %s already exists!", name );
        m_Shaders[name] = pShader;
    }

    void ShaderLibrary::Add( const Ref< Shader > &pShader )
    {
        auto &name = pShader->GetName();
        Add( name, pShader );
    }

    Ref< Shader > ShaderLibrary::Load( const std::string &filePath, const BufferLayout &bufferLayout )
    {
        auto pShader = Shader::Create( filePath, bufferLayout );
        Add( pShader );
        return pShader;
    }

    Ref< Shader >
    ShaderLibrary::Load( const std::string &name, const std::string &filePath, const BufferLayout &bufferLayout )
    {
        auto pShader = Shader::Create( filePath, bufferLayout );
        Add( name, pShader );
        return pShader;
    }

    Ref< Shader > ShaderLibrary::Get( const std::string &name )
    {
        SM_ASSERT( Exists( name ), "ShaderLibrary::Add > Shader: %s not found!", name );
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists( const std::string &name ) const
    {
        return m_Shaders.find( name ) != m_Shaders.end();
    }
}