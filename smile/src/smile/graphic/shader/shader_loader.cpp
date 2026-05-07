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
    struct ShaderHeader final
    {
        Uint32 Magic;
        Uint32 Version;
        Uint32 BlobOffset;
        Uint32 BlobSize;
        Uint32 YamlOffset;
        Uint32 YamlSize;
    };

    ShaderLoader::ShaderLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::Asset > ShaderLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return Load( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    memory::Ref< ShaderAsset > ShaderLoader::Load( const fs::Path &path ) const
    {
        if ( path.IsEmpty() )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.GetExtension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: wrong file extension" );
            return nullptr;
        }

        auto pShaderAsset = memory::CreateRef< ShaderAsset >();

        ShaderSerializer serializer{ pShaderAsset };
        if ( !serializer.Deserialize( path ) )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Deserialization failed" );
            return nullptr;
        }

        return pShaderAsset;
    }
}