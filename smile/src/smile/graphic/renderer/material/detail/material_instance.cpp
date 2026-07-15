/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material_instance.h"

#include "material.h"

namespace smile::graphic::detail
{
    MaterialInstance::MaterialInstance( const MaterialInstanceHandle handle,
        const MaterialDescriptor &desc,
        const MaterialHandle materialHandle ) noexcept
        : m_Handle{ handle },
          m_Descriptor{ desc },
          m_MaterialHandle{ materialHandle },
          m_DirtyFlags{ { DirtyFlags::Parameter, DirtyFlags::Texture } }
    {
    }

    void MaterialInstance::Clear() noexcept
    {
        // m_Params.Clear();
    }

    void MaterialInstance::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        m_Descriptor.SetParameter( name, data );
        m_DirtyFlags.Set( DirtyFlags::Parameter );
    }

    const MaterialParameterValue &MaterialInstance::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.GetParameter( name );
    }

    void MaterialInstance::SetTextureBinding( const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_Descriptor.SetTextureBinding( name, texture, samplerDesc );
        m_DirtyFlags.Set( DirtyFlags::Texture );
    }

    const MaterialTextureBinding &MaterialInstance::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_Descriptor.GetTextureBinding( name );
    }
}