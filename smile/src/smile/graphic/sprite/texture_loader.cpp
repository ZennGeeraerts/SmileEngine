/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "texture_loader.h"

#include "texture_asset.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "smile/graphic/renderer/render_engine.h"

namespace smile::graphic
{
    TextureLoader::TextureLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
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
            SM_LOG_WARNING( "TextureLoader::LoadTexture > Failed to load world: the path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.extension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "TextureLoader::LoadTexture > Failed to load world: wrong file extention" );
            return nullptr;
        }

        memory::Ref< Texture > pTexture = RenderEngine::GetRenderSystem().GetResourceManager().CreateTexture( path );
        memory::Ref< TextureAsset > pTextureAsset = memory::CreateRef< TextureAsset >( pTexture );

        return pTextureAsset;
    }
}