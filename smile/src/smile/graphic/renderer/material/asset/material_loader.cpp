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
 * @file        material_loader.cpp
 * @author      Zenn Geeraerts
 * @created     19 Januari 2026
 * @brief       Loads a material asset
 */
#include "smpch.h"
#include "material_loader.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "material_serializer.h"

namespace smile::graphic
{
    MaterialLoader::MaterialLoader( TextureManager &textureManager, ShaderLibrary &shaderLib ) noexcept
        : m_TextureManager{ textureManager }, m_ShaderLibrary{ shaderLib }
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::Asset > MaterialLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadMaterial( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    MaterialAsset::Ref MaterialLoader::LoadMaterial( const std::filesystem::path &path ) const
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "MaterialLoader::LoadMaterial > Failed to load material: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.extension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "MaterialLoader::LoadMaterial > Failed to load material: wrong file extension" );
            return nullptr;
        }

        auto pMaterialAsset = memory::CreateRef< MaterialAsset >();

        MaterialSerializer serializer{ pMaterialAsset, m_TextureManager, m_ShaderLibrary };
        if ( !serializer.Deserialize( fs::Path{ path.string().c_str() } ) )
        {
            SM_LOG_WARNING( "MaterialLoader::LoadMaterial > Deserialization failed" );
            return nullptr;
        }

        return pMaterialAsset;
    }
}