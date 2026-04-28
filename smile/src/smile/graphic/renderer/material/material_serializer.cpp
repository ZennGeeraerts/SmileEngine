/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material_serializer.cpp
 * @author      Zenn Geeraerts
 * @created     16 Januari 2026
 * @brief       Serializes a material asset to a file
 */
#include "smpch.h"
#include "material_serializer.h"

#include "smile/core/yaml/string.h"
#include "smile/core/yaml/math.h"
#include "smile/core/fs/file.h"
#include "smile/graphic/shader/shader_library.h"
#include "smile/graphic/renderer/sprite/texture_manager.h"

namespace smile::graphic
{
    MaterialSerializer::MaterialSerializer( MaterialAsset::Ref materialAsset,
        TextureManager &textureManager,
        ShaderLibrary &shaderLib ) noexcept
        : m_MaterialAsset{ materialAsset }, m_TextureManager{ textureManager }, m_ShaderLib{ shaderLib }
    {
    }

    void MaterialSerializer::Serialize( const fs::Path &filePath ) const
    {
        fs::File outputFile{ filePath };
        if ( !outputFile.OpenOutput( stream::OpeningModeFlags{} ) )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", filePath );
            return;
        }

        yaml::Emitter yamlOutput{};
        yamlOutput << YAML::BeginMap;
        {
            const MaterialDescriptor &desc = m_MaterialAsset->GetDescriptor();
            const MaterialLayout &layout = m_MaterialAsset->GetLayout();

            yamlOutput << YAML::Key << "Name" << YAML::Value << m_MaterialAsset->GetName();
            yamlOutput << YAML::Key << "VertexShader" << YAML::Value << desc.ShaderProgram->GetVertexShader()->m_Handle;
            yamlOutput << YAML::Key << "PixelShader" << YAML::Value << desc.ShaderProgram->GetPixelShader()->m_Handle;

            yamlOutput << YAML::Key << "Parameters";

            yamlOutput << YAML::BeginMap;

            for ( const auto &param : layout.Parameters )
            {
                auto it = desc.Parameters.FindItemAtKey( param.Name );
                if ( it != desc.Parameters.end() )
                {
                    switch ( param.Type )
                    {
                        case MaterialParameterType::Bool:
                            yamlOutput << YAML::Key << param.Name << YAML::Value << std::get< bool >( it.GetItem() );
                            break;
                        case MaterialParameterType::Int:
                            yamlOutput << YAML::Key << param.Name << YAML::Value << std::get< int >( it.GetItem() );
                            break;
                        case MaterialParameterType::Float:
                            yamlOutput << YAML::Key << param.Name << YAML::Value << std::get< float >( it.GetItem() );
                            break;
                        case MaterialParameterType::Float2:
                            yamlOutput << YAML::Key << param.Name << YAML::Value
                                       << std::get< DirectX::XMFLOAT2 >( it.GetItem() );
                            break;
                        case MaterialParameterType::Float3:
                            yamlOutput << YAML::Key << param.Name << YAML::Value
                                       << std::get< DirectX::XMFLOAT3 >( it.GetItem() );
                            break;
                    }
                }
            }

            yamlOutput << YAML::EndMap;

            yamlOutput << YAML::Key << "Textures";

            yamlOutput << YAML::BeginMap;

            for ( const auto &texture : layout.Textures )
            {
                auto it = desc.TextureBindings.FindItemAtKey( texture.Name );
                if ( it != desc.TextureBindings.end() )
                {
                    const MaterialTextureBinding &binding = it.GetItem();
                    auto textureAsset = m_TextureManager.GetTexture( binding.Texture );
                    yamlOutput << YAML::Key << texture.Name << YAML::Value;

                    yamlOutput << YAML::BeginMap;
                    {
                        yamlOutput << YAML::Key << "Texture" << YAML::Value << textureAsset->m_Handle;

                        yamlOutput << YAML::Key << "Sampler";
                        yamlOutput << YAML::BeginMap;
                        {
                            yamlOutput << YAML::Key << "Filtering" << YAML::Value
                                       << static_cast< Uint32 >( binding.SamplerDescriptor.Filtering );

                            yamlOutput << YAML::Key << "AddressingU" << YAML::Value
                                       << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingU );

                            yamlOutput << YAML::Key << "AddressingV" << YAML::Value
                                       << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingV );

                            yamlOutput << YAML::Key << "AddressingW" << YAML::Value
                                       << static_cast< Uint32 >( binding.SamplerDescriptor.AddressingW );
                        }
                        yamlOutput << YAML::EndMap;
                    }
                    yamlOutput << YAML::EndMap;
                }
            }

            yamlOutput << YAML::EndMap;
        }
        yamlOutput << YAML::EndMap;

        outputFile.WriteText( yamlOutput.c_str() );

        outputFile.Close();
    }

    bool MaterialSerializer::Deserialize( const fs::Path &filePath )
    {
        fs::File file{ filePath };
        if ( !file.OpenInput() )
        {
            SM_LOG_WARNING(
                "MaterialSerializer::Deserialize > Failed to load material: cannot open material file: {}", filePath );

            return false;
        }

        primitive::String yamlContent{};
        file.ReadByteArray( yamlContent.GetData(), file.GetSize() );

        yaml::Node data;
        try
        {
            data = YAML::Load( yamlContent.GetData() );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_ERROR( "Failed to load material: {}", e.what() );
            return false;
        }

        if ( !data["Name"] )
            return false;

        m_MaterialAsset->m_Name = data["Name"].as< primitive::String >();

        if ( !data["VertexShader"] )
            return false;

        asset::AssetHandle vertexShaderHandle = data["VertexShader"].as< Uint64 >();
        ShaderAsset::Ref pVertexShader = m_ShaderLib.GetShader( vertexShaderHandle );

        if ( !data["PixelShader"] )
            return false;

        asset::AssetHandle pixelShaderHandle = data["PixelShader"].as< Uint64 >();
        ShaderAsset::Ref pPixelShader = m_ShaderLib.GetShader( pixelShaderHandle );

        auto program = Program::Create( pVertexShader, pPixelShader );

        BuildMaterialLayoutAndDescriptor(
            m_TextureManager, program, m_MaterialAsset->m_Layout, m_MaterialAsset->m_Descriptor );

        if ( data["Parameters"] )
        {
            for ( const auto &param : m_MaterialAsset->m_Layout.Parameters )
            {
                YAML::Node valueNode = data["Parameters"][param.Name];
                if ( !valueNode )
                    continue;

                switch ( param.Type )
                {
                    case MaterialParameterType::Bool:
                    {
                        m_MaterialAsset->SetParameter( param.Name, valueNode.as< bool >() );
                        break;
                    }

                    case MaterialParameterType::Int:
                    {
                        m_MaterialAsset->SetParameter( param.Name, valueNode.as< int >() );
                        break;
                    }

                    case MaterialParameterType::Float:
                    {
                        m_MaterialAsset->SetParameter( param.Name, valueNode.as< float >() );
                        break;
                    }

                    case MaterialParameterType::Float2:
                    {
                        m_MaterialAsset->SetParameter( param.Name, valueNode.as< DirectX::XMFLOAT2 >() );
                        break;
                    }

                    case MaterialParameterType::Float3:
                    {
                        m_MaterialAsset->SetParameter( param.Name, valueNode.as< DirectX::XMFLOAT3 >() );
                        break;
                    }
                }
            }
        }

        if ( data["Textures"] )
        {
            for ( const auto &texture : m_MaterialAsset->m_Layout.Textures )
            {
                YAML::Node valueNode = data["Textures"][texture.Name];
                if ( !valueNode )
                    continue;

                if ( !valueNode["Texture"] || !valueNode["Sampler"] )
                    continue;

                asset::AssetHandle textureAssetHandle = valueNode["Texture"].as< Uint64 >();

                rhi::SamplerDescriptor samplerDesc{};

                samplerDesc.Filtering =
                    static_cast< rhi::SamplerFiltering >( valueNode["Sampler"]["Filtering"].as< Uint32 >() );

                samplerDesc.AddressingU =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingU"].as< Uint32 >() );

                samplerDesc.AddressingV =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingV"].as< Uint32 >() );

                samplerDesc.AddressingW =
                    static_cast< rhi::SamplerAddressing >( valueNode["Sampler"]["AddressingW"].as< Uint32 >() );

                auto textureAsset = m_TextureManager.GetTexture( textureAssetHandle );

                m_MaterialAsset->SetTextureBinding( texture.Name, textureAsset->GetTexture(), samplerDesc );
            }
        }

        return true;
    }
}