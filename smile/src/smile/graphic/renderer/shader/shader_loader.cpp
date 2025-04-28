/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "shader_loader.h"

#include "shader_asset.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"

#include <fstream>

namespace smile::graphic
{
    ShaderLoader::ShaderLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::Asset > ShaderLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadShader( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    memory::Ref< ShaderAsset > ShaderLoader::LoadShader( const std::filesystem::path &path ) const
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.extension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: wrong file extension" );
            return nullptr;
        }

        constexpr Uint32 expectedMagic = 0x53484452; // 'SHDR'

        std::ifstream file{ path, std::ios::binary };
        if ( !file )
        {
            SM_LOG_WARNING(
                "ShaderLoader::LoadShader > Failed to load shader: cannot open shader file: {}", path.string() );
            return nullptr;
        }

        Uint32 magic = 0;
        Uint32 version = 0;
        Uint32 blobSize = 0;
        Uint32 yamlSize = 0;

        file.read( reinterpret_cast< char * >( &magic ), sizeof( Uint32 ) );
        file.read( reinterpret_cast< char * >( &version ), sizeof( Uint32 ) );
        file.read( reinterpret_cast< char * >( &blobSize ), sizeof( Uint32 ) );
        file.read( reinterpret_cast< char * >( &yamlSize ), sizeof( Uint32 ) );

        if ( magic != expectedMagic )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: invalid shader file magic" );
            return nullptr;
        }

        std::vector< Byte > byteCode{};
        byteCode.resize( blobSize );

        file.read( reinterpret_cast< char * >( byteCode.data() ), blobSize );

        //std::string yamlText{ yamlSize, '\0' };
        //file.read( &yamlText[0], yamlSize );

        //shaderFile.metadata = YAML::Load( yamlText );

        return nullptr;
    }
}