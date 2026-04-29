/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material_instance.h"

#include "material.h"

namespace smile::graphic
{
    MaterialInstance::MaterialInstance( ID id, const MaterialDescriptor &desc, const Material &material ) noexcept
        : m_ID{ id },
          m_Descriptor{ desc },
          m_Material{ material },
          m_DirtyFlags{ { DirtyFlags::Parameter, DirtyFlags::Texture } }
    {
    }

    void MaterialInstance::Clear() noexcept
    {
        // m_Params.Clear();
    }

    void MaterialInstance::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        if ( !m_Descriptor.Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        // TODO: Once we have our own vector class that supports operator==
        /*if ( m_Descriptor.Parameters[name] == data )
            return;*/

        m_Descriptor.Parameters[name] = data;
        m_DirtyFlags.Set( DirtyFlags::Parameter );
    }

    const MaterialParameterValue &MaterialInstance::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.Parameters.GetItemAtKey( name );
    }

    void MaterialInstance::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        if ( !m_Descriptor.TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        MaterialTextureBinding textureBinding{ texture, samplerDesc };

        if ( m_Descriptor.TextureBindings[name] == textureBinding )
            return;

        m_Descriptor.TextureBindings[name] = std::move( textureBinding );
        m_DirtyFlags.Set( DirtyFlags::Texture );
    }

    const MaterialTextureBinding &MaterialInstance::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_Descriptor.TextureBindings.GetItemAtKey( name );
    }
}