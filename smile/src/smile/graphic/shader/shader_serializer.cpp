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
#include "smile/core/fs/file.h"

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
            node["SemanticIndex"] = element.SemanticIndex;
            node["Format"] = rhi::GetFormatInfo( element.FormatType ).Name;
            inputs.push_back( node );
        }

        yaml["InputSignature"] = inputs;

        yaml::Node outputs;
        for ( const rhi::BufferElement &element : reflectionData.OutputSignature )
        {
            yaml::Node node;
            node["SemanticName"] = element.Name;
            node["SemanticIndex"] = element.SemanticIndex;
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
        for ( const auto &[key, binding] : reflectionData.ShaderResourceBindings )
        {
            YAML::Node resNode;
            resNode["Name"] = binding.Name;
            resNode["Type"] = rhi::GetResourceTypeInfo( binding.Element.Type ).Name;
            resNode["Slot"] = binding.Element.Slot;
            resources.push_back( resNode );
        }

        yaml["Resources"] = resources;

        return yaml;
    }

    static bool DeserializeReflectionData( const primitive::String &yamlContent, ShaderReflectionData &reflectionData )
    {
        yaml::Node data;
        try
        {
            data = YAML::Load( yamlContent.GetData() );
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
                element.Name = entry["SemanticName"].as< primitive::String >();
                element.SemanticIndex = entry["SemanticIndex"].as< Index >();
                const rhi::FormatInfo &formatInfo = rhi::GetFormatInfo( entry["Format"].as< primitive::String >() );
                element.FormatType = formatInfo.Format;
                element.Size = formatInfo.BytesPerBlock * formatInfo.BlockSize;

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

                const rhi::BindingLayoutElement element{ slot, type };

                const ResourceBindingKey key{ slot, ResourceTypeToBindingType( type ) };
                NamedBindingLayoutElement namedElement{ element, name };

                reflectionData.ShaderResourceBindings.Insert( key, std::move( namedElement ) );
            }
        }

        return true;
    }

    ShaderSerializer::ShaderSerializer( ShaderAsset::Ref pShaderAsset ) : m_pShaderAsset{ pShaderAsset }
    {
    }

    void ShaderSerializer::Serialize( const fs::Path &filePath ) const
    {
        yaml::Emitter reflectionDataOutput{};
        reflectionDataOutput << SerializeReflectionData( m_pShaderAsset->m_ReflectionData );

        fs::File outputFile{ filePath };
        if ( !outputFile.OpenOutput( stream::OpeningModeFlags{} ) )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", filePath );
            return;
        }

        const Index nameOffset = sizeof( ShaderHeader );
        const Count nameSize = m_pShaderAsset->GetName().GetCharCount();

        const Index blobOffset = nameOffset + nameSize;
        const Count blobSize = m_pShaderAsset->GetByteCode().GetItemCount();

        const Index reflectionDataOffset = blobOffset + blobSize;
        const Count reflectionDataSize = static_cast< Count >( reflectionDataOutput.size() );

        // Header
        outputFile.WriteScalar< Uint32 >( g_ShaderFileMagic );
        outputFile.WriteScalar< Uint32 >( g_ShaderFileVersion );
        outputFile.WriteScalar< Index >( nameOffset );
        outputFile.WriteScalar< Count >( nameSize );
        outputFile.WriteScalar< Index >( blobOffset );
        outputFile.WriteScalar< Count >( blobSize );
        outputFile.WriteScalar< Index >( reflectionDataOffset );
        outputFile.WriteScalar< Count >( reflectionDataSize );

        // Name
        outputFile.WriteText( m_pShaderAsset->GetName() );

        // Blob
        outputFile.WriteByteArray( m_pShaderAsset->GetByteCode().GetData(), blobSize );

        // ReflectionData
        outputFile.WriteText( { reflectionDataOutput.c_str(), reflectionDataSize } );

        outputFile.Close();
    }

    bool ShaderSerializer::Deserialize( const fs::Path &filePath )
    {
        fs::File file{ filePath };
        if ( !file.OpenInput() )
        {
            SM_LOG_WARNING(
                "ShaderSerializer::Deserialize > Failed to load shader: cannot open shader file: {}", filePath );

            return false;
        }

        ShaderHeader header{};
        file.ReadByteArray( &header, sizeof( ShaderHeader ) );

        if ( header.Magic != g_ShaderFileMagic )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: invalid shader file magic" );

            file.Close();

            return false;
        }

        if ( header.Version != g_ShaderFileVersion )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: unexpected version" );

            file.Close();

            return false;
        }

        if ( header.NameOffset + header.NameSize > file.GetSize() ||
             header.BlobOffset + header.BlobSize > file.GetSize() ||
             header.YamlOffset + header.YamlSize > file.GetSize() )
        {
            SM_LOG_WARNING( "ShaderSerializer::Deserialize > Failed to load shader: shader file blob or YAML section "
                            "out of bounds" );

            file.Close();

            return false;
        }

        file.SetIndex( header.NameOffset );
        m_pShaderAsset->m_Name.SetCharCount( header.NameSize );
        file.ReadByteArray( m_pShaderAsset->m_Name.GetData(), header.NameSize );

        file.SetIndex( header.BlobOffset );
        m_pShaderAsset->m_ByteCode.SetItemCount( header.BlobSize );
        file.ReadByteArray( m_pShaderAsset->m_ByteCode.GetData(), header.BlobSize );

        file.SetIndex( header.YamlOffset );
        primitive::String yamlContent{};
        yamlContent.SetCharCount( header.YamlSize );
        file.ReadByteArray( yamlContent.GetData(), header.YamlSize );

        if ( !DeserializeReflectionData( yamlContent, m_pShaderAsset->m_ReflectionData ) )
        {
            SM_LOG_WARNING(
                "ShaderSerializer::Deserialize > Failed to load shader: cannot deserialize reflection data" );

            file.Close();

            return false;
        }

        file.Close();

        return true;
    }
}