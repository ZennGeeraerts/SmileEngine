#pragma once

#include "shader.h"

namespace smile::graphic
{
    class ShaderLibrary final
    {
      public:
        void add( const std::string &name, const Ref< Shader > &shader );
        void add( const Ref< Shader > &shader );
        Ref< Shader > load( const std::string &file_path, const BufferLayout &buffer_layout );
        Ref< Shader > load( const std::string &name, const std::string &file_path, const BufferLayout &buffer_layout );

        Ref< Shader > get( const std::string &name );
        bool exists( const std::string &name ) const;

      private:
        std::unordered_map< std::string, Ref< Shader > > shaders;
    };
}