/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

namespace smile::graphic
{
    Material::Material( const MaterialDescriptor &desc ) : m_Descriptor{ desc }
    {
    }

    void Material::Clear()
    {
        // m_Params.Clear();
    }

    void Material::SetParameter( const primitive::StringView name, const MaterialParameter::Value &data )
    {
        if ( !m_Descriptor.Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        auto &param = m_Descriptor.Parameters.GetItemAtKey( name );
        param.Data = data;

        m_DirtyFlags.Set( DirtyFlags::Parameter );
    }

    MaterialParameter::Value Material::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.Parameters.GetItemAtKey( name ).Data;
    }

    void Material::SetTextureBinding( const primitive::StringView name, Texture::Ref texture )
    {
        if ( !m_Descriptor.TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "Material::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        auto &textureBinding = m_Descriptor.TextureBindings.GetItemAtKey( name );
        textureBinding.Texture = texture;

        m_DirtyFlags.Set( DirtyFlags::Texture );
    }

    Texture::Ref Material::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_Descriptor.TextureBindings.GetItemAtKey( name ).Texture;
    }
}