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
 * @file        material.cpp
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Holds the data of a material and its default instance
 */
#include "smpch.h"
#include "material.h"

namespace smile::graphic
{
    Material::Material( const primitive::String &name, const MaterialLayout &layout )
        : m_Name{ name }, m_Layout{ layout }
    {
    }

    void Material::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        m_DefaultInstance->SetParameter( name, data );
    }

    const MaterialParameterValue &Material::GetParameter( const primitive::StringView name ) const
    {
        return m_DefaultInstance->GetParameter( name );
    }

    void Material::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_DefaultInstance->SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &Material::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_DefaultInstance->GetTextureBinding( name );
    }
}