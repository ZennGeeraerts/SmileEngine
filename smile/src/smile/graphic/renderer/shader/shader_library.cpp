/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "shader_library.h"

#include "shader_asset.h"
#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"

#include "smile/core/asset/asset_manager.h"

namespace smile::graphic
{
    memory::Ref< ShaderAsset > ShaderLibrary::GetShader( asset::AssetHandle handle )
    {
        memory::Ref< ShaderAsset > pShaderAsset = asset::AssetManager::GetAsset< ShaderAsset >( handle );

        if ( pShaderAsset )
        {
            m_Shaders.Insert( pShaderAsset->GetName(), pShaderAsset );
            return pShaderAsset;
        }

        SM_LOG_WARNING( "ShaderLibrary::GetShader > Could not load shader: {}", static_cast< Uint64 >( handle ) );

        return nullptr;
    }

    memory::Ref< ShaderAsset > ShaderLibrary::GetShader( const primitive::StringView shaderName ) const
    {
        if ( Exists( shaderName ) )
        {
            return m_Shaders[shaderName];
        }

        SM_LOG_WARNING( "ShaderLibrary::GetShader > Could not load shader with name: {}", shaderName );

        return nullptr;
    }

    memory::Ref< ShaderAsset > ShaderLibrary::LoadShader( const std::filesystem::path &path )
    {
        memory::Ref< ShaderAsset > pShaderAsset = m_ShaderLoader.LoadShader( path );

        if ( pShaderAsset )
        {
            m_Shaders.Insert( pShaderAsset->GetName(), pShaderAsset );
            return pShaderAsset;
        }

        SM_LOG_WARNING( "ShaderLibrary::GetShader > Could not load shader: {}", path.string() );

        return nullptr;
    }

    bool ShaderLibrary::Exists( const primitive::StringView shaderName ) const
    {
        return m_Shaders.HasItemAtKey( shaderName );
    }
}