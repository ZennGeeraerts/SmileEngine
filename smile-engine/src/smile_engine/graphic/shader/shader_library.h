/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "shader.h"

namespace smile::graphic
{
    class ShaderLibrary final
    {
      public:
        void Add( const std::string &name, const Ref< Shader > &pShader );
        void Add( const Ref< Shader > &pShader );
        Ref< Shader > Load( const std::string &filePath, const BufferLayout &bufferLayout );
        Ref< Shader > Load( const std::string &name, const std::string &filePath, const BufferLayout &bufferLayout );

        Ref< Shader > Get( const std::string &name );
        bool Exists( const std::string &name ) const;

      private:
        std::unordered_map< std::string, Ref< Shader > > m_Shaders;
    };
}