/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"
#include "shader_loader.h"

namespace smile::graphic
{
    class ShaderLibrary final
    {
      public:
        memory::Ref< ShaderAsset > GetShader( asset::AssetHandle handle );
        memory::Ref< ShaderAsset > GetShaderByName( const primitive::StringView shaderName );
        memory::Ref< ShaderAsset > GetShader( const std::filesystem::path &path );

        bool Exists( const primitive::StringView shaderName ) const;

      private:
        ShaderLoader m_ShaderLoader;
        std::unordered_map< primitive::String, memory::Ref< ShaderAsset > > m_Shaders;
        std::unordered_map< memory::Ref< ShaderAsset >, primitive::String > m_ShadersToNameMap;
    };
}