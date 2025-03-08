/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader.h"
#include "memory/ref.h"

namespace smile::graphic
{
    class ShaderLibrary final
    {
      public:
        void Add( const std::string &name, const memory::Ref< Shader > &pShader );
        void Add( const memory::Ref< Shader > &pShader );
        memory::Ref< Shader > Load( const std::string &filePath, const BufferLayout &bufferLayout );
        memory::Ref< Shader >
        Load( const std::string &name, const std::string &filePath, const BufferLayout &bufferLayout );

        memory::Ref< Shader > Get( const std::string &name );
        bool Exists( const std::string &name ) const;

      private:
        std::unordered_map< std::string, memory::Ref< Shader > > m_Shaders;
    };
}