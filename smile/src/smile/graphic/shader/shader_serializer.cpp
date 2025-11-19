/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        shader_serializer.cpp
 * @author      Zenn Geeraerts
 * @created     18 November 2025
 * @brief       Serialization and deserialization of CPU shaders
 */
#include "smpch.h"
#include "shader_serializer.h"

#include "smile/core/yaml/string.h"

#include <fstream>

namespace smile::graphic
{
    constexpr Uint32 g_ShaderFileMagic{ 0x53484452 }; // 'SHDR'
    constexpr Uint32 g_ShaderFileVersion{ 1 };

    struct ShaderHeader final
    {
        Uint32 Magic;
        Uint32 Version;

        Index NameOffset;
        Count NameSize;

        Index BlobOffset;
        Count BlobSize;

        Index YamlOffset;
        Count YamlSize;
    };

    static yaml::Node SerializeReflectionData( const ShaderReflectionData &reflectionData )
    {
        yaml::Node yaml;

        yaml["EntryPoint"] = reflectionData.EntryPoint;
        yaml["TargetProfile"] = reflectionData.TargetProfile;
        yaml["BlobFormat"] = ShaderBlobFormatToString( reflectionData.BlobFormat );

        yaml::Node inputs;
        for ( const rhi::BufferElement &element : reflectionData.InputSignature )
        {
            yaml::Node node;
            node["SemanticName"] = element.Name;
            node["Format"] = rhi::GetFormatInfo( element.FormatType ).Name;
            inputs.push_back( node );
        }

        yaml["InputSignature"] = inputs;

        yaml::Node outputs;
        for ( const rhi::BufferElement &element : reflectionData.OutputSignature )
        {
            yaml::Node node;
            node["SemanticName"] = element.Name;
            node["Format"] = rhi::GetFormatInfo( element.FormatType ).Name;
            outputs.push_back( node );
        }

        yaml["OutputSignature"] = outputs;

        yaml::Node cbuffers;
        for ( const auto &[name, desc] : reflectionData.ConstantBufferDescs )
        {
            yaml::Node cbNode;
            cbNode["Name"] = name;
            cbNode["Size"] = desc.GetSize();
            cbNode["Variables"] = yaml::Node{ YAML::NodeType::Sequence };

            for ( const ConstantBufferItem &item : desc )
            {
                YAML::Node varNode;
                varNode["Name"] = item.Name;
                varNode["Type"] = GetConstantTypeInfo( item.Type ).Name;
                varNode["Size"] = item.Size;
                varNode["Offset"] = item.Offset;
                cbNode["Variables"].push_back( varNode );
            }

            cbuffers.push_back( cbNode );
        }

        yaml["ConstantBuffers"] = cbuffers;

        yaml::Node resources;
        for ( const auto &[name, binding] : reflectionData.ShaderResourceBindings )
        {
            YAML::Node resNode;
            resNode["Name"] = name;
            resNode["Type"] = rhi::GetResourceTypeInfo( binding.Type ).Name;
            resNode["Slot"] = binding.Slot;
            resources.push_back( resNode );
        }

        yaml["Resources"] = resources;

        return yaml;
    }

    static bool DeserializeReflectionData( const std::string &yamlContent, ShaderReflectionData &reflectionData )
    {
        yaml::Node data;
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

        reflectionData.BlobFormat = ShaderBlobFormatFromString( data["BlobFormat"].as< primitive::String >() );

        auto parseSignature = []( const yaml::Node &node ) -> rhi::BufferLayout
        {
            rhi::BufferLayout signature{};
            for ( const auto &entry : node )
            {
                rhi::BufferElement element;
                element.Name = entry["SemanticName"].as< std::string >();
                auto val = entry["Format"].as< std::string >();
                element.FormatType = rhi::GetFormatInfo( entry["Format"].as< primitive::String >() ).Format;

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
                const primitive::String name = resourceNode["Name"].as< primitive::String >();

                const rhi::ResourceType type =
                    rhi::GetResourceTypeInfo( resourceNode["Type"].as< primitive::String >() ).Type;
                const Uint32 slot = resourceNode["Slot"].as< Uint32 >();

                rhi::BindingLayoutElement element{ slot, type };

                reflectionData.ShaderResourceBindings.Insert( name, std::move( element ) );
            }
        }

        return true;
    }

    ShaderSerializer::ShaderSerializer( ShaderAsset::Ref pShaderAsset ) : m_pShaderAsset{ pShaderAsset }
    {
    }

    void ShaderSerializer::Serialize( const std::filesystem::path &filePath ) const
    {
        yaml::Emitter reflectionDataOutput{};
        reflectionDataOutput << SerializeReflectionData( m_pShaderAsset->m_ReflectionData );

        std::ofstream outStream{ filePath, std::ios::binary };
        if ( !outStream )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", filePath.string() );
            return;
        }

        const Index nameOffset = sizeof( ShaderHeader );
        const Count nameSize = m_pShaderAsset->GetName().GetCharCount();

        const Index blobOffset = nameOffset + nameSize;
        const Count blobSize = m_pShaderAsset->GetByteCode().GetItemCount();

        const Index reflectionDataOffset = blobOffset + blobSize;
        const Count reflectionDataSize = static_cast< Count >( reflectionDataOutput.size() );

        // Header
        outStream.write( reinterpret_cast< const char * >( &g_ShaderFileMagic ), sizeof( Uint32 ) );
        outStream.write( reinterpret_cast< const char * >( &g_ShaderFileVersion ), sizeof( Uint32 ) );
        outStream.write( reinterpret_cast< const char * >( &nameOffset ), sizeof( Index ) );
        outStream.write( reinterpret_cast< const char * >( &nameSize ), sizeof( Count ) );
        outStream.write( reinterpret_cast< const char * >( &blobOffset ), sizeof( Index ) );
        outStream.write( reinterpret_cast< const char * >( &blobSize ), sizeof( Count ) );
        outStream.write( reinterpret_cast< const char * >( &reflectionDataOffset ), sizeof( Index ) );
        outStream.write( reinterpret_cast< const char * >( &reflectionDataSize ), sizeof( Count ) );

        // Name
        outStream.write( m_pShaderAsset->GetName().GetData(), nameSize );

        // Blob
        outStream.write( reinterpret_cast< const char * >( m_pShaderAsset->GetByteCode().GetData() ), blobSize );

        // ReflectionData
        outStream.write( reflectionDataOutput.c_str(), reflectionDataSize );
    }

    bool ShaderSerializer::Deserialize( const std::filesystem::path &filePath )
    {
        std::ifstream file{ filePath, std::ios::binary };
        if ( !file )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: cannot open shader file: {}",
                filePath.string() );
            return false;
        }

        std::vector< Byte > fileData{ ( std::istreambuf_iterator< char >{ file } ), {} };
        ShaderHeader header{};
        // TODO: Use binary stream class when we have it
        std::memcpy( &header, fileData.data(), sizeof( ShaderHeader ) );

        if ( header.Magic != g_ShaderFileMagic )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: invalid shader file magic" );
            return false;
        }

        if ( header.Version != g_ShaderFileVersion )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: unexpected version" );
            return false;
        }

        if ( header.NameOffset + header.NameSize > fileData.size() ||
             header.BlobOffset + header.BlobSize > fileData.size() ||
             header.YamlOffset + header.YamlSize > fileData.size() )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: shader file blob or YAML section "
                            "out of bounds" );
            return false;
        }

        m_pShaderAsset->m_Name = primitive::String{
            reinterpret_cast< const char * >( fileData.data() + header.NameOffset ), header.NameSize };

        const Byte *pBlobStart = fileData.data() + header.BlobOffset;
        m_pShaderAsset->m_ByteCode = { pBlobStart, header.BlobSize };

        std::string yamlContent{
            reinterpret_cast< const char * >( fileData.data() + header.YamlOffset ), header.YamlSize };

        if ( !DeserializeReflectionData( yamlContent, m_pShaderAsset->m_ReflectionData ) )
        {
            SM_LOG_WARNING(
                "ShaderSerializer::Deserialize > Failed to load shader: cannot deserialize reflection data" );
            return false;
        }

        return true;
    }
}