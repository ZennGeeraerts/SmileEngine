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
 * @file        material_asset_descriptor.cpp
 * @author      Zenn Geeraerts
 * @created     11 May 2026
 * @brief       Asset-level material descriptor; holds TextureAsset refs instead of GPU resources
 */
#include "smpch.h"
#include "material_asset_descriptor.h"

namespace smile::graphic
{
    void MaterialAssetDescriptor::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        if ( !Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "MaterialAsset::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        Parameters[name] = data;
    }

    const MaterialParameterValue &MaterialAssetDescriptor::GetParameter( const primitive::StringView name ) const
    {
        return Parameters.GetItemAtKey( name );
    }

    void MaterialAssetDescriptor::SetTextureBinding( const primitive::StringView name,
        TextureAsset::Ref textureAsset,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        if ( !TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "MaterialAsset::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        TextureBindings[name] = { textureAsset, samplerDesc };
    }

    const MaterialAssetTextureBinding &MaterialAssetDescriptor::GetTextureBinding(
        const primitive::StringView name ) const
    {
        return TextureBindings.GetItemAtKey( name );
    }
}
