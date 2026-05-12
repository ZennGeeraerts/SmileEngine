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

namespace smile::graphic
{
    MaterialInstanceAsset::Ref MaterialInstanceManager::CreateMaterialInstance( const primitive::String &name,
        MaterialAsset::Ref material,
        const MaterialAssetDescriptor &desc )
    {
        SM_ASSERT( !m_MaterialInstances.HasItemAtKey( name ) );

        MaterialInstanceAsset::Ref materialInstance =
            memory::CreateRef< MaterialInstanceAsset >( name, material, desc );

        m_MaterialInstances.Insert( name, materialInstance );

        return materialInstance;
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

    MaterialInstanceAsset::Ref MaterialInstanceManager::LoadMaterialInstance( const fs::Path &path )
    {
        MaterialInstanceAsset::Ref materialInstanceAsset = m_MaterialInstanceLoader.Load( path );

        if ( materialInstanceAsset )
        {
            m_MaterialInstances.Insert( materialInstanceAsset->GetName(), materialInstanceAsset );
            return materialInstanceAsset;
        }

        SM_LOG_WARNING( "MaterialInstanceManager::LoadMaterialInstance > Could not load material instance: {}", path );

        return nullptr;
    }
}