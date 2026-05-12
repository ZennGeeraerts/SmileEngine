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
        SM_ASSERT( !Contains( name ) );

        MaterialInstanceAsset::Ref materialInstance =
            memory::CreateRef< MaterialInstanceAsset >( name, material, desc );

        Insert( materialInstance );

        return materialInstance;
    }
}