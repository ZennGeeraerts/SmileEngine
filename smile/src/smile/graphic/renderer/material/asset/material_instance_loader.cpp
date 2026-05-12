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
 * @file        material_instance_loader.cpp
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Loads a material instance asset
 */
#include "smpch.h"
#include "material_instance_loader.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "material_instance_serializer.h"

namespace smile::graphic
{
    MaterialInstanceLoader::MaterialInstanceLoader() noexcept
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::BaseAsset > MaterialInstanceLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return Load( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    MaterialInstanceAsset::Ref MaterialInstanceLoader::Load( const fs::Path &path ) const
    {
        if ( path.IsEmpty() )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::Load > Failed to load material instances: the "
                            "path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.GetExtension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::Load > Failed to load material instance: wrong "
                            "file extension" );
            return nullptr;
        }

        auto pMaterialInstanceAsset = memory::CreateRef< MaterialInstanceAsset >();

        MaterialInstanceSerializer serializer{ pMaterialInstanceAsset };

        if ( !serializer.Deserialize( path ) )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::Load > Deserialization failed" );
            return nullptr;
        }

        return pMaterialInstanceAsset;
    }
}