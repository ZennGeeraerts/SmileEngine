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
        m_System->SetMaterialInstanceParameter( m_Handle, name, value );
    }

    const MaterialParameterValue &MaterialInstance::GetParameter( const primitive::StringView name ) const
    {
        return m_System->GetMaterialInstanceParameter( m_Handle, name );
    }

    void MaterialInstance::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_System->SetMaterialInstanceTextureBinding( m_Handle, name, texture, samplerDesc );
    }

    const MaterialTextureBinding &MaterialInstance::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_System->GetMaterialInstanceTextureBinding( m_Handle, name );
    }

    bool MaterialInstance::IsValid() const noexcept
    {
        return m_System->IsMaterialInstanceValid( m_Handle );
    }

    Material MaterialInstance::GetMaterial() const
    {
        const auto materialHandle = m_System->GetMaterialHandle( m_Handle );

        return { materialHandle, m_System };
    }
}