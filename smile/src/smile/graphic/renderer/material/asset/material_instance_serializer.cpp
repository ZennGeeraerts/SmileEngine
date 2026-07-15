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
 * @file        material_instance_serializer.h
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Serializes a material instance asset to a file
 */
#include "smpch.h"
#include "material_instance_serializer.h"

#include "smile/core/yaml/string.h"
#include "smile/core/fs/file.h"
#include "smile/core/asset/asset_manager.h"

#include "material_serializer_utils.h"

namespace smile::graphic
{
    MaterialInstanceSerializer::MaterialInstanceSerializer( MaterialInstanceAsset::Ref materialInstanceAsset ) noexcept
        : m_MaterialInstanceAsset{ materialInstanceAsset }
    {
    }

    void MaterialInstanceSerializer::Serialize( const fs::Path &filePath ) const
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
            const MaterialAssetDescriptor &desc = m_MaterialInstanceAsset->GetDescriptor();
            const MaterialLayout &layout = m_MaterialInstanceAsset->m_MaterialAsset->GetLayout();

            yamlOutput << YAML::Key << "Name" << YAML::Value << m_MaterialInstanceAsset->GetName();
            yamlOutput << YAML::Key << "Material" << YAML::Value << m_MaterialInstanceAsset->m_MaterialAsset->m_Handle;
            yamlOutput << YAML::Key << "VertexShader" << YAML::Value << desc.ShaderProgram->GetVertexShader()->m_Handle;
            yamlOutput << YAML::Key << "PixelShader" << YAML::Value << desc.ShaderProgram->GetPixelShader()->m_Handle;

            SerializeMaterialAssetDescriptor( layout, desc, yamlOutput );
        }
        yamlOutput << YAML::EndMap;

        outputFile.WriteText( yamlOutput.c_str() );

        outputFile.Close();
    }

    bool MaterialInstanceSerializer::Deserialize( const fs::Path &filePath )
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

        m_MaterialInstanceAsset->m_Name = data["Name"].as< primitive::String >();

        if ( !data["Material"] )
            return false;

        auto materialAssetHandle = data["Material"].as< Uint64 >();
        m_MaterialInstanceAsset->m_MaterialAsset = asset::AssetManager::GetAsset< MaterialAsset >( materialAssetHandle );

        if ( !data["VertexShader"] )
            return false;

        asset::AssetHandle vertexShaderHandle = data["VertexShader"].as< Uint64 >();
        ShaderAsset::Ref pVertexShader = asset::AssetManager::GetAsset< ShaderAsset >( vertexShaderHandle );

        if ( !data["PixelShader"] )
            return false;

        asset::AssetHandle pixelShaderHandle = data["PixelShader"].as< Uint64 >();
        ShaderAsset::Ref pPixelShader = asset::AssetManager::GetAsset< ShaderAsset >( pixelShaderHandle );

        m_MaterialInstanceAsset->m_Descriptor.ShaderProgram = Program::Create( pVertexShader, pPixelShader );

        DeserializeMaterialAssetDescriptor(
            data,
            m_MaterialInstanceAsset->m_MaterialAsset->GetLayout(),
            m_MaterialInstanceAsset->m_Descriptor );

        return true;
    }
}