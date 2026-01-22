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

#include "smile/core/yaml/yaml.h"
#include "smile/core/fs/file.h"
#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/shader/shader_library.h"

namespace smile::graphic
{
    MaterialSerializer::MaterialSerializer( MaterialAsset::Ref pMaterialAsset ) : m_pMaterialAsset{ pMaterialAsset }
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
            yamlOutput << YAML::Key << "VertexShader" << YAML::Value << m_pMaterialAsset->GetVertexShader()->m_Handle;
            yamlOutput << YAML::Key << "PixelShader" << YAML::Value << m_pMaterialAsset->GetPixelShader()->m_Handle;
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

        auto &shaderLibrary = RenderEngine::GetShaderLibrary();

        if ( !data["VertexShader"] )
            return false;

        asset::AssetHandle vertexShaderHandle = data["VertexShader"].as< asset::AssetHandle >();
        ShaderAsset::Ref pVertexShader = shaderLibrary.GetShader( vertexShaderHandle );

        if ( !data["PixelShader"] )
            return false;

        asset::AssetHandle pixelShaderHandle = data["PixelShader"].as< asset::AssetHandle >();
        ShaderAsset::Ref pPixelShader = shaderLibrary.GetShader( pixelShaderHandle );

        m_pMaterialAsset->SetShaders( pVertexShader, pPixelShader );

        return true;
    }
}