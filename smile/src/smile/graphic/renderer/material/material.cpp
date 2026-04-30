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
 * @created     30 April 2026
 * @brief       Public facing API, object oriented material class
 */
#include "smpch.h"
#include "material.h"

#include "material_system.h"

namespace smile::graphic
{
    Material::Material( const detail::MaterialHandle handle, detail::MaterialSystem *system ) noexcept
        : m_Handle{ handle }, m_System{ system }
    {
    }

    void Material::SetParameter( const primitive::StringView name, const MaterialParameterValue &value )
    {
        GetDefaultInstance().SetParameter( name, value );
    }

    const MaterialParameterValue &Material::GetParameter( const primitive::StringView name ) const
    {
        return GetDefaultInstance().GetParameter( name );
    }

    void Material::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        GetDefaultInstance().SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &Material::GetTextureBinding( const primitive::StringView name ) const
    {
        return GetDefaultInstance().GetTextureBinding( name );
    }

    MaterialInstance Material::GetDefaultInstance() const
    {
        const auto handle = m_System->GetMaterial( m_Handle ).GetDefaultInstance();

        return { handle, m_System };
    }

    const MaterialLayout &Material::GetLayout() const
    {
        return m_System->GetMaterial( m_Handle ).GetLayout();
    }
}