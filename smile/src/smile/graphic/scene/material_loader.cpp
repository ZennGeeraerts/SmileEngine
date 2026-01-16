#include "smpch.h"
#include "material_loader.h"

#include "smile/core/asset/asset_importer.h"

namespace smile::graphic
{
    MaterialLoader::MaterialLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    MaterialAsset::Ref MaterialLoader::LoadMaterial( const std::filesystem::path &path ) const
    {
    }
}