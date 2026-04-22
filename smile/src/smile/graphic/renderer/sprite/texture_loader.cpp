/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "texture_loader.h"

#include "texture_asset.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/resource/reader/image_reader.h"

namespace smile::graphic
{
    TextureLoader::TextureLoader() noexcept
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    void TextureLoader::Initialize( ResourceManager *resourceManager ) noexcept
    {
        m_ResourceManager = resourceManager;
    }

    memory::Ref< asset::Asset > TextureLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadTexture( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    memory::Ref< TextureAsset > TextureLoader::LoadTexture( const std::filesystem::path &path ) const
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "TextureLoader::LoadTexture > Failed to load texture: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.extension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "TextureLoader::LoadTexture > Failed to load texture: wrong file extension" );
            return nullptr;
        }

        Image::Ref image = ImageReader::Read( path );
        Texture::Ref texture = m_ResourceManager->CreateTexture2D( image, false );
        TextureAsset::Ref textureAsset = memory::CreateRef< TextureAsset >( texture );

        return textureAsset;
    }
}