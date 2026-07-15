/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "shader_loader.h"

#include "shader_serializer.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"

namespace smile::graphic
{
    ShaderLoader::ShaderLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::BaseAsset > ShaderLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return Load( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    ShaderAsset::Ref ShaderLoader::Load( const fs::Path &path ) const
    {
        if ( path.IsEmpty() )
        {
            SM_LOG_WARNING( "ShaderLoader::Load > Failed to load shader: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.GetExtension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "ShaderLoader::Load > Failed to load shader: wrong file extension" );
            return nullptr;
        }

        auto pShaderAsset = memory::CreateRef< ShaderAsset >();

        ShaderSerializer serializer{ pShaderAsset };
        if ( !serializer.Deserialize( path ) )
        {
            SM_LOG_WARNING( "ShaderLoader::Load > Deserialization failed" );
            return nullptr;
        }

        return pShaderAsset;
    }
}