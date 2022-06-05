#include "smpch.h"
#include "shader_library.h"

namespace smile
{
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