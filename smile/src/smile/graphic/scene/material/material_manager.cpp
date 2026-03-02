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
 * @file        material_manager.cpp
 * @author      Zenn Geeraerts
 * @created     27 February 2026
 * @brief       Manages material assets
 */
#include "smpch.h"
#include "material_manager.h"

#include "smile/core/asset/asset_manager.h"
#include "smile/graphic/sprite/texture_manager.h"

namespace smile::graphic
{
    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        const MaterialLayout &layout,
        const MaterialDescriptor &desc )
    {
        SM_ASSERT( !m_Materials.HasItemAtKey( name ) );

        MaterialAsset::Ref material = memory::CreateRef< MaterialAsset >( name, layout, desc );
        m_Materials.Insert( name, material );
        return material;
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name, Program::ConstRef program )
    {
        MaterialLayout layout{};
        MaterialDescriptor desc{};

        BuildMaterialLayoutAndDescriptor( program, layout, desc );

        return CreateMaterial( name, layout, desc );
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        ShaderAsset::ConstRef vertexShader,
        ShaderAsset::ConstRef pixelShader )
    {
        auto program = Program::Create( vertexShader, pixelShader );
        return CreateMaterial( name, program );
    }

    MaterialAsset::Ref MaterialManager::GetMaterial( asset::AssetHandle handle )
    {
        MaterialAsset::Ref pMaterialAsset = asset::AssetManager::GetAsset< MaterialAsset >( handle );

        if ( pMaterialAsset )
        {
            m_Materials.Insert( pMaterialAsset->GetName(), pMaterialAsset );
            return pMaterialAsset;
        }

        SM_LOG_WARNING( "MaterialManager::GetMaterial > Could not find material: {}", static_cast< Uint64 >( handle ) );

        return nullptr;
    }

    MaterialAsset::Ref MaterialManager::GetMaterial( const primitive::StringView name ) const
    {
        if ( m_Materials.HasItemAtKey( name ) )
        {
            return m_Materials[name];
        }

        SM_LOG_WARNING( "MaterialManager::GetMaterial > Could not find material with name: {}", name );

        return nullptr;
    }

    MaterialAsset::Ref MaterialManager::LoadMaterial( const std::filesystem::path &path )
    {
        MaterialAsset::Ref pMaterialAsset = m_MaterialLoader.LoadMaterial( path );

        if ( pMaterialAsset )
        {
            m_Materials.Insert( pMaterialAsset->GetName(), pMaterialAsset );
            return pMaterialAsset;
        }

        SM_LOG_WARNING( "MaterialManager::LoadMaterial > Could not load material: {}", path.string() );

        return nullptr;
    }
}