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

#include "smile/graphic/sprite/texture_manager.h"

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

    MaterialParameterValue Material::GetParameter( const primitive::StringView name ) const
    {
        return m_DefaultInstance->GetParameter( name );
    }

    void Material::SetTextureBinding( const primitive::StringView name,
        Texture::ConstRef texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        m_DefaultInstance->SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &Material::GetTextureBinding( const primitive::StringView name ) const
    {
        return m_DefaultInstance->GetTextureBinding( name );
    }

    static MaterialParameterType ConstantTypeToMaterialParamType( ConstantType constantType )
    {
        switch ( constantType )
        {
            case ConstantType::Float:
                return MaterialParameterType::Float;
            case ConstantType::Float2:
                return MaterialParameterType::Float2;
            case ConstantType::Float3:
                return MaterialParameterType::Float3;
            case ConstantType::Int:
                return MaterialParameterType::Int;
            case ConstantType::Bool:
                return MaterialParameterType::Bool;

            default:
                SM_ASSERT( false, "Not supported" );
        }
    }

    void BuildMaterialLayoutAndDescriptor( Program::ConstRef program, MaterialLayout &layout, MaterialDescriptor &desc )
    {
        const auto &cbDesc = program->GetConstantBufferDescriptor( "Material" );
        for ( const auto &cbItem : cbDesc )
        {
            MaterialLayout::Parameter parameter{
                cbItem.Name, ConstantTypeToMaterialParamType( cbItem.Type ), cbItem.Offset, cbItem.Size };

            layout.Parameters.PushBack( std::move( parameter ) );
            desc.Parameters.Insert( cbItem.Name, {} );
        }

        const auto &resources = program->GetResources();
        for ( const auto &res : resources )
        {
            if ( res.NamedElement.Element.Type == rhi::ResourceType::Texture_SRV ||
                 res.NamedElement.Element.Type == rhi::ResourceType::Texture_UAV )
            {
                MaterialLayout::Texture textureBinding{ res.NamedElement.Name, res.NamedElement.Element.Slot };

                layout.Textures.PushBack( std::move( textureBinding ) );

                auto fallbackTexture = TextureManager::GetInstance().GetFallBackTexture()->GetTexture();
                rhi::SamplerDescriptor fallbackSampler{};

                desc.TextureBindings.Insert( res.NamedElement.Name, { fallbackTexture, fallbackSampler } );
            }
            else if ( res.NamedElement.Name == "Material" )
            {
                layout.CbSlot = res.NamedElement.Element.Slot;
            }
        }

        layout.CbSize = cbDesc.GetSize();
        desc.ShaderProgram = program;
    }
}