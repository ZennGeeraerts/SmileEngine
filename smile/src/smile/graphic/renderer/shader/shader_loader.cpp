/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "shader_loader.h"

#include "shader_asset.h"
#include "constant_type.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"

#include <yaml-cpp/yaml.h>

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
        constexpr Uint32 expectedVersion = 1;

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

        if ( header.Version != expectedVersion )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: unexpected version" );
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

        ShaderReflectionData reflectionData{};
        if ( !DeserializeReflectionData( yamlContent, reflectionData ) )
        {
            SM_LOG_WARNING( "ShaderLoader::LoadShader > Failed to load shader: cannot deserialize reflection data" );
            return nullptr;
        }

        auto pShaderAsset = memory::CreateRef< ShaderAsset >( byteCode, reflectionData );
        return pShaderAsset;
    }

    bool ShaderLoader::DeserializeReflectionData( const std::string &yamlContent,
        ShaderReflectionData &reflectionData ) const
    {
        YAML::Node data;
        try
        {
            data = YAML::Load( yamlContent );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_ERROR( "Failed to load shader reflection data: {}", e.what() );
            return false;
        }

        if ( !data["EntryPoint"] )
            return false;

        reflectionData.EntryPoint = data["EntryPoint"].as< primitive::String >();

        if ( !data["TargetProfile"] )
            return false;

        reflectionData.TargetProfile = data["TargetProfile"].as< primitive::String >();

        if ( !data["BlobFormat"] )
            return false;

        reflectionData.BlobFormat = ShaderBlobFormatFromString( data["BlobFormat"].as< std::string >() );

        auto parseSignature = []( const YAML::Node &node ) -> rhi::BufferLayout
        {
            rhi::BufferLayout signature{};
            for ( const auto &entry : node )
            {
                rhi::BufferElement element;
                element.Name = entry["SemanticName"].as< std::string >();
                auto val = entry["Format"].as< std::string >();
                element.FormatType = rhi::GetFormatInfo( entry["Format"].as< std::string >() ).Format;

                signature.AddElement( element );
            }
            return signature;
        };

        if ( data["InputSignature"] )
            reflectionData.InputSignature = parseSignature( data["InputSignature"] );

        if ( data["OutputSignature"] )
            reflectionData.OutputSignature = parseSignature( data["OutputSignature"] );

        if ( data["ConstantBuffers"] )
        {
            for ( const auto &bufferNode : data["ConstantBuffers"] )
            {
                ConstantBufferDescriptor buffer;

                SM_ASSERT( bufferNode["Name"] );
                primitive::String bufferName = bufferNode["Name"].as< primitive::String >();

                if ( bufferNode["Variables"] )
                {
                    for ( const auto &varNode : bufferNode["Variables"] )
                    {
                        ConstantBufferItem item;
                        item.Name = varNode["Name"].as< primitive::String >();
                        item.Type = GetConstantTypeInfo( varNode["Type"].as< primitive::String >() ).Type;
                        item.Size = varNode["Size"].as< Count >();
                        item.ItemCount = 1; // TODO: Support item count

                        buffer.Add( item );
                    }
                }

                reflectionData.ConstantBufferDescs.Insert( bufferName, std::move( buffer ) );
            }
        }

        if ( data["Resources"] )
        {
            for ( const auto &resourceNode : data["Resources"] )
            {
                ShaderResourceBinding resource;
                resource.Name = resourceNode["Name"].as< primitive::String >();
                resource.Type = rhi::GetResourceTypeInfo( resourceNode["Type"].as< primitive::String >() ).Type;
                resource.BindPoint = resourceNode["BindPoint"].as< Uint32 >();
                resource.BindCount = resourceNode["BindCount"].as< Uint32 >();

                reflectionData.ShaderResourceBindings.EmplaceBack( std::move( resource ) );
            }
        }

        return true;
    }
}