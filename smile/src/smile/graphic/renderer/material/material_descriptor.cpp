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
 * @file        material_descriptor.cpp
 * @author      Zenn Geeraerts
 * @created     13 April 2026
 * @brief       Values for parameters and texture bindings of a material
 */
#include "smpch.h"
#include "material_descriptor.h"

namespace smile::graphic
{
    void MaterialDescriptor::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        if ( !Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        // TODO: Once we have our own vector class that supports operator==
        /*if ( m_Descriptor.Parameters[name] == data )
            return;*/

        Parameters[name] = data;
    }

    const MaterialParameterValue &MaterialDescriptor::GetParameter( const primitive::StringView name ) const
    {
        return Parameters.GetItemAtKey( name );
    }

    void MaterialDescriptor::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        if ( !TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        MaterialTextureBinding textureBinding{ texture, samplerDesc };

        if ( TextureBindings[name] == textureBinding )
            return;

        TextureBindings[name] = std::move( textureBinding );
    }

    const MaterialTextureBinding &MaterialDescriptor::GetTextureBinding( const primitive::StringView name ) const
    {
        return TextureBindings.GetItemAtKey( name );
    }
}