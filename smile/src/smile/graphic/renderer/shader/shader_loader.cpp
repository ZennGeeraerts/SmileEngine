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

        std::vector< Byte > fileData{ ( std::istreambuf_iterator< char >{ file } ), {} };
        ShaderHeader header{};
        // TODO: Use binary stream class when we have it
        std::memcpy( &header, fileData.data(), sizeof( ShaderHeader ) );

        if ( header.Magic != expectedMagic )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: invalid shader file magic" );
            return nullptr;
        }

        if ( header.BlobOffset + header.BlobSize > fileData.size() ||
             header.YamlOffset + header.YamlSize > fileData.size() )
        {
            SM_LOG_WARNING(
                "ShaderLoader::LoadShader > Failed to load shader: shader file blob or YAML section out of bounds" );
            return nullptr;
        }

        const Byte *pBlobStart = fileData.data() + header.BlobOffset;
        std::vector< Byte > byteCode{ pBlobStart, pBlobStart + header.BlobSize };

        std::string yamlContent{
            reinterpret_cast< const char * >( fileData.data() + header.YamlOffset ), header.YamlSize };

        return nullptr;
    }
}