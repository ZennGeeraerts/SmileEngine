/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "shader_loader.h"
#include "shader_asset.h"

namespace smile::graphic
{
    class ShaderLibrary final
    {
      public:
        memory::Ref< ShaderAsset > GetShader( asset::AssetHandle handle );
        memory::Ref< ShaderAsset > GetShader( const primitive::StringView shaderName ) const;
        memory::Ref< ShaderAsset > LoadShader( const fs::Path &path );

        bool Exists( const primitive::StringView shaderName ) const;

      private:
        ShaderLoader m_ShaderLoader;
        primitive::HashMap< primitive::String, memory::Ref< ShaderAsset > > m_Shaders;
    };
}