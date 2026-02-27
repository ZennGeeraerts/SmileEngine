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
 * @file        material_manager.cpp
 * @author      Zenn Geeraerts
 * @created     27 February 2026
 * @brief       Manages material assets
 */
#include "smpch.h"
#include "material_manager.h"

#include "smile/core/asset/asset_manager.h"
#include "smile/graphic/sprite/texture_manager.h"

namespace smile::graphic
{
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

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        const MaterialLayout &layout,
        const MaterialDescriptor &desc )
    {
        SM_ASSERT( !m_Materials.HasItemAtKey( name ) );

        MaterialAsset::Ref material = memory::CreateRef< MaterialAsset >( name, layout, desc );
        m_Materials.Insert( name, material );
        return material;
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name, Program::ConstRef program )
    {
        MaterialLayout layout{};
        MaterialDescriptor desc{};

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

        return CreateMaterial( name, layout, desc );
    }

    MaterialAsset::Ref MaterialManager::CreateMaterial( const primitive::String &name,
        ShaderAsset::ConstRef vertexShader,
        ShaderAsset::ConstRef pixelShader )
    {
        auto program = Program::Create( vertexShader, pixelShader );
        return CreateMaterial( name, program );
    }

    MaterialAsset::Ref MaterialManager::GetMaterial( asset::AssetHandle handle )
    {
        MaterialAsset::Ref pMaterialAsset = asset::AssetManager::GetAsset< MaterialAsset >( handle );

        if ( pMaterialAsset )
        {
            m_Materials.Insert( pMaterialAsset->GetName(), pMaterialAsset );
            return pMaterialAsset;
        }

        SM_LOG_WARNING( "MaterialManager::GetMaterial > Could not find material: {}", static_cast< Uint64 >( handle ) );

        return nullptr;
    }

    MaterialAsset::Ref MaterialManager::GetMaterial( const primitive::StringView name ) const
    {
        if ( m_Materials.HasItemAtKey( name ) )
        {
            return m_Materials[name];
        }

        SM_LOG_WARNING( "MaterialManager::GetMaterial > Could not find material with name: {}", name );

        return nullptr;
    }

    MaterialAsset::Ref MaterialManager::LoadMaterial( const std::filesystem::path &path )
    {
        MaterialAsset::Ref pMaterialAsset = m_MaterialLoader.LoadMaterial( path );

        if ( pMaterialAsset )
        {
            m_Materials.Insert( pMaterialAsset->GetName(), pMaterialAsset );
            return pMaterialAsset;
        }

        SM_LOG_WARNING( "MaterialManager::LoadMaterial > Could not load material: {}", path.string() );

        return nullptr;
    }
}