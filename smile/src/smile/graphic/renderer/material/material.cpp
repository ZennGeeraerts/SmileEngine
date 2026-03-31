/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

namespace smile::graphic
{
    Material::Material( ID id, const MaterialLayout &layout, const MaterialDescriptor &desc )
        : m_ID{ id },
          m_Layout{ layout },
          m_Descriptor{ desc },
          m_DirtyFlags{ { DirtyFlags::Parameter, DirtyFlags::Texture } }
    {
    }

    void Material::Clear()
    {
        // m_Params.Clear();
    }

    void Material::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        if ( !m_Descriptor.Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        m_Descriptor.Parameters[name] = data;
        m_DirtyFlags.Set( DirtyFlags::Parameter );
    }

    MaterialParameterValue Material::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.Parameters.GetItemAtKey( name );
    }

    void Material::SetTextureBinding( const primitive::StringView name, Texture::ConstRef texture )
    {
        if ( !m_Descriptor.TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        m_Descriptor.TextureBindings[name] = texture;
        m_DirtyFlags.Set( DirtyFlags::Texture );
    }

    Texture::ConstRef Material::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_Descriptor.TextureBindings.GetItemAtKey( name );
    }
}