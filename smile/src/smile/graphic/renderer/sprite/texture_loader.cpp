/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "texture_loader.h"

#include "texture_asset.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "smile/graphic/resource/reader/image_reader.h"

namespace smile::graphic
{
    TextureLoader::TextureLoader() noexcept
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::BaseAsset > TextureLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return Load( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    TextureAsset::Ref TextureLoader::Load( const fs::Path &path ) const
    {
        if ( path.IsEmpty() )
        {
            SM_LOG_WARNING( "TextureLoader::Load > Failed to load texture: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.GetExtension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "TextureLoader::Load > Failed to load texture: wrong file extension" );
            return nullptr;
        }

        Image::Ref image = ImageReader::Read( path );
        TextureAsset::Ref textureAsset = memory::CreateRef< TextureAsset >( std::move( image ) );

        return textureAsset;
    }
}