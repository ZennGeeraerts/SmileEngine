#include "smpch.h"
#include "shader_library.h"

namespace smile::renderer
{
    void ShaderLibrary::add( const std::string &name, const Ref< Shader > &shader )
    {
        SM_ASSERT( !exists( name ), "ShaderLibrary::add > Shader: %s already exists!", name );
        shaders[name] = shader;
    }

    void ShaderLibrary::add( const Ref< Shader > &shader )
    {
        auto &name = shader->getName();
        add( name, shader );
    }

    Ref< Shader > ShaderLibrary::load( const std::string &file_path, const BufferLayout &buffer_layout )
    {
        auto shader = Shader::create( file_path, buffer_layout );
        add( shader );
        return shader;
    }

    Ref< Shader >
    ShaderLibrary::load( const std::string &name, const std::string &file_path, const BufferLayout &buffer_layout )
    {
        auto shader = Shader::create( file_path, buffer_layout );
        add( name, shader );
        return shader;
    }

    Ref< Shader > ShaderLibrary::get( const std::string &name )
    {
        SM_ASSERT( exists( name ), "ShaderLibrary::add > Shader: %s not found!", name );
        return shaders[name];
    }

    bool ShaderLibrary::exists( const std::string &name ) const
    {
        return shaders.find( name ) != shaders.end();
    }
}