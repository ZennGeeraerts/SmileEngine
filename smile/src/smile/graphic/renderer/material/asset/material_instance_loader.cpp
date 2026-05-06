#include "smpch.h"
#include "material_instance_loader.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "material_instance_serializer.h"

namespace smile::graphic
{
    MaterialInstanceLoader::MaterialInstanceLoader( TextureManager &textureManager,
        ShaderLibrary &shaderLib,
        MaterialManager &materialManager ) noexcept
        : m_TextureManager{ textureManager }, m_ShaderLibrary{ shaderLib }, m_MaterialManager{ materialManager }
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::Asset > MaterialInstanceLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadMaterialInstance( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    MaterialInstanceAsset::Ref MaterialInstanceLoader::LoadMaterialInstance( const std::filesystem::path &path ) const
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::LoadMaterialInstance > Failed to load material instances: the "
                            "path was empty" );
            return nullptr;
        }

        if ( std::find( m_Extensions.begin(), m_Extensions.end(), path.extension() ) == m_Extensions.end() )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::LoadMaterialInstance > Failed to load material instance: wrong "
                            "file extension" );
            return nullptr;
        }

        auto pMaterialInstanceAsset = memory::CreateRef< MaterialInstanceAsset >();

        MaterialInstanceSerializer serializer{ pMaterialInstanceAsset, m_TextureManager, m_ShaderLibrary, m_MaterialManager };
        if ( !serializer.Deserialize( fs::Path{ path.string().c_str() } ) )
        {
            SM_LOG_WARNING( "MaterialInstanceLoader::LoadMaterialInstance > Deserialization failed" );
            return nullptr;
        }

        return pMaterialInstanceAsset;
    }
}