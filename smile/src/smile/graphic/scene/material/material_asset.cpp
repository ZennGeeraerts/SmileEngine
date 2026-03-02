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
 * @file        material_asset.h
 * @author      Zenn Geeraerts
 * @created     16 Januari 2026
 * @brief       Asset for material
 */
#include "smpch.h"
#include "material_asset.h"

#include "smile/graphic/sprite/texture_manager.h"

namespace smile::graphic
{
    void MaterialAsset::SetParameter( const primitive::StringView name, const MaterialParameterValue &data )
    {
        if ( !m_Descriptor.Parameters.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "MaterialAsset::SetParameter > Could not find material parameter with name: {}", name );
            return;
        }

        m_Descriptor.Parameters[name] = data;
    }

    MaterialParameterValue MaterialAsset::GetParameter( const primitive::StringView name ) const
    {
        return m_Descriptor.Parameters.GetItemAtKey( name );
    }

    void MaterialAsset::SetTextureBinding( const primitive::StringView name, TextureAsset::Ref texture )
    {
        if ( !m_Descriptor.TextureBindings.HasItemAtKey( name ) )
        {
            SM_LOG_WARNING( "MaterialAsset::SetTextureBinding > Could not find texture binding with name: {}", name );
            return;
        }

        m_Descriptor.TextureBindings[name] = texture->GetTexture();
    }

    TextureAsset::Ref MaterialAsset::GetTextureBinding( const primitive::StringView name ) const
    {
        auto texture = m_Descriptor.TextureBindings.GetItemAtKey( name );
        return TextureManager::GetInstance().GetTexture( texture );
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

                desc.TextureBindings.Insert(
                    res.NamedElement.Name, TextureManager::GetInstance().GetFallBackTexture() );
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