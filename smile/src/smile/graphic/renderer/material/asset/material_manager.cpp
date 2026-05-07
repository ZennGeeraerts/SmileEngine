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

namespace smile::graphic
{
    MaterialManager::MaterialManager( TextureManager &textureManager, ShaderLibrary &shaderLibrary ) noexcept
        : m_MaterialLoader{ textureManager, shaderLibrary }, m_TextureManager{ textureManager }
    {
    }

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

        BuildMaterialLayoutAndDescriptor( m_TextureManager, program, layout, desc );

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

    MaterialAsset::Ref MaterialManager::LoadMaterial( const fs::Path &path )
    {
        MaterialAsset::Ref pMaterialAsset = m_MaterialLoader.Load( path );

        if ( pMaterialAsset )
        {
            m_Materials.Insert( pMaterialAsset->GetName(), pMaterialAsset );
            return pMaterialAsset;
        }

        SM_LOG_WARNING( "MaterialManager::LoadMaterial > Could not load material: {}", path );

        return nullptr;
    }
}