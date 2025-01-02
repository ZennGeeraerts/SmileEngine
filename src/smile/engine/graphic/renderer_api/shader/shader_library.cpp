/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "shader_library.h"

#include "smile/engine/graphic/renderer/render_command.h"

namespace smile::graphic
{
    void ShaderLibrary::Add( const std::string &name, const Ref< Shader > &pShader )
    {
        SM_ASSERT( !Exists( name ), "ShaderLibrary::Add > Shader: %s already exists!", name );
        m_Shaders[name] = pShader;
    }

    void ShaderLibrary::Add( const Ref< Shader > &pShader )
    {
        Add( pShader->Name, pShader );
    }

    Ref< Shader > ShaderLibrary::Load( const std::string &filePath, const BufferLayout &bufferLayout )
    {
        auto pShader = RenderCommand::GetGraphicsDevice()->CreateShader( filePath, bufferLayout );
        Add( pShader );
        return pShader;
    }

    Ref< Shader >
    ShaderLibrary::Load( const std::string &name, const std::string &filePath, const BufferLayout &bufferLayout )
    {
        auto pShader = RenderCommand::GetGraphicsDevice()->CreateShader( filePath, bufferLayout );
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