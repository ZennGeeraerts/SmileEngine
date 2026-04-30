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
 * @file        material_instance.cpp
 * @author      Zenn Geeraerts
 * @created     30 April 2026
 * @brief       Public facing API, object oriented material instance class
 */
#include "smpch.h"
#include "material.h"

#include "material_system.h"

namespace smile::graphic
{
    MaterialInstance::MaterialInstance( const detail::MaterialInstanceHandle handle,
        detail::MaterialSystem *system ) noexcept
        : m_Handle{ handle }, m_System{ system }
    {
    }

    void MaterialInstance::SetParameter( const primitive::StringView name, const MaterialParameterValue &value )
    {
        m_System->GetMaterialInstance( m_Handle ).SetParameter( name, value );
    }

    const MaterialParameterValue &MaterialInstance::GetParameter( const primitive::StringView name ) const
    {
        return m_System->GetMaterialInstance( m_Handle ).GetParameter( name );
    }

    void MaterialInstance::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_System->GetMaterialInstance( m_Handle ).SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &MaterialInstance::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_System->GetMaterialInstance( m_Handle ).GetTextureBinding( name );
    }

    Material MaterialInstance::GetMaterial() const
    {
        const auto materialHandle = m_System->GetMaterialInstance( m_Handle ).GetMaterialHandle();

        return { materialHandle, m_System };
    }
}