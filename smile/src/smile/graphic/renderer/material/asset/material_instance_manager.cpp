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
 * @file        material_instance_manager.cpp
 * @author      Zenn Geeraerts
 * @created     6 May 2026
 * @brief       Manages material instance assets
 */
#include "smpch.h"
#include "material_instance_manager.h"

#include "smile/core/asset/asset_manager.h"

namespace smile::graphic
{
    MaterialInstanceManager::MaterialInstanceManager( TextureManager &textureManager,
        ShaderLibrary &shaderLibrary,
        MaterialManager &materialManager ) noexcept
        : m_MaterialInstanceLoader{ textureManager, shaderLibrary, materialManager }, m_TextureManager{ textureManager }
    {
    }

    MaterialInstanceAsset::Ref MaterialInstanceManager::CreateMaterialInstance( const primitive::String &name,
        MaterialAsset::Ref material,
        const MaterialDescriptor &desc )
    {
        SM_ASSERT( !m_MaterialInstances.HasItemAtKey( name ) );

        MaterialInstanceAsset::Ref materialInstance =
            memory::CreateRef< MaterialInstanceAsset >( name, material, desc );

        m_MaterialInstances.Insert( name, materialInstance );

        return materialInstance;
    }

    MaterialInstanceAsset::Ref MaterialInstanceManager::GetMaterialInstance( asset::AssetHandle handle )
    {
        MaterialInstanceAsset::Ref materialInstanceAsset =
            asset::AssetManager::GetAsset< MaterialInstanceAsset >( handle );

        if ( materialInstanceAsset )
        {
            m_MaterialInstances.Insert( materialInstanceAsset->GetName(), materialInstanceAsset );
            return materialInstanceAsset;
        }

        SM_LOG_WARNING( "MaterialInstanceManager::GetMaterialInstance > Could not find material instance: {}",
            static_cast< Uint64 >( handle ) );

        return nullptr;
    }

    MaterialInstanceAsset::Ref MaterialInstanceManager::GetMaterialInstance( const primitive::StringView name ) const
    {
        if ( m_MaterialInstances.HasItemAtKey( name ) )
        {
            return m_MaterialInstances[name];
        }

        SM_LOG_WARNING(
            "MaterialInstanceManager::GetMaterialInstance > Could not find material instance with name: {}", name );

        return nullptr;
    }

    MaterialInstanceAsset::Ref MaterialInstanceManager::LoadMaterialInstance( const std::filesystem::path &path )
    {
        MaterialInstanceAsset::Ref materialInstanceAsset = m_MaterialInstanceLoader.LoadMaterialInstance( path );

        if ( materialInstanceAsset )
        {
            m_MaterialInstances.Insert( materialInstanceAsset->GetName(), materialInstanceAsset );
            return materialInstanceAsset;
        }

        SM_LOG_WARNING(
            "MaterialInstanceManager::LoadMaterialInstance > Could not load material instance: {}", path.string() );

        return nullptr;
    }
}