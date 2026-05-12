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

#include "smile/graphic/renderer/sprite/texture_manager.h"

namespace smile::graphic
{
    MaterialManager::MaterialManager( TextureManager &textureManager ) noexcept
        : AssetProvider{ textureManager }, m_TextureManager{ textureManager }
    {
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        const MaterialLayout &layout,
        const MaterialAssetDescriptor &desc )
    {
        SM_ASSERT( !Contains( name ) );

        MaterialAsset::Ref material = memory::CreateRef< MaterialAsset >( name, layout, desc );
        Insert( material );
        return material;
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name, Program::ConstRef program )
    {
        MaterialLayout layout{};
        MaterialAssetDescriptor desc{};

        BuildMaterialLayoutAndDescriptor( m_TextureManager.GetFallback(), program, layout, desc );

        return CreateMaterial( name, layout, desc );
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        ShaderAsset::ConstRef vertexShader,
        ShaderAsset::ConstRef pixelShader )
    {
        auto program = Program::Create( vertexShader, pixelShader );
        return CreateMaterial( name, program );
    }
}