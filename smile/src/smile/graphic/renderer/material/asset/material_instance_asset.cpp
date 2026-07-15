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
 * @file        material_instance_asset.cpp
 * @author      Zenn Geeraerts
 * @created     5 May 2026
 * @brief       Asset for material instance
 */
#include "smpch.h"
#include "material_instance_asset.h"

namespace smile::graphic
{
    void MaterialInstanceAsset::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        m_Descriptor.SetParameter( name, data );
    }

    const MaterialParameterValue &MaterialInstanceAsset::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.GetParameter( name );
    }

    void MaterialInstanceAsset::SetTextureBinding( const primitive::StringView name,
        TextureAsset::Ref texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_Descriptor.SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialAssetTextureBinding &MaterialInstanceAsset::GetTextureBinding(
        const primitive::StringView name ) const
    {
        return m_Descriptor.GetTextureBinding( name );
    }
}