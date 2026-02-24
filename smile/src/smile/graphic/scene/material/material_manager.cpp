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
 * @created     29 Januari 2026
 * @brief       Manages gpu resources and updates of material
 */
#include "smpch.h"
#include "material_manager.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/common/memory/memory.h"

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

    Material::Ref MaterialManager::CreateMaterial( const MaterialLayout &layout, const MaterialDescriptor &desc )
    {
        Material::ID id = m_IDManager.CreateHandle();
        Material::Ref material = memory::CreateRef< Material >( id, desc );

        MaterialData data;
        data.ShaderProgram = desc.ShaderProgram;

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

        const auto &cbDesc = desc.ShaderProgram->GetConstantBufferDescriptor( "Material" );
        data.ConstantBuffer = resourceManager.CreateConstantBuffer( cbDesc );

        return material;
    }

    Material::Ref MaterialManager::CreateMaterial( Program::ConstRef program )
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
                desc.TextureBindings.Insert( res.NamedElement.Name, {} ); // TODO: Get default texture
            }
            else if ( res.NamedElement.Name == "Material" )
            {
                layout.CbSlot = res.NamedElement.Element.Slot;
            }
        }

        layout.CbSize = cbDesc.GetSize();
        desc.ShaderProgram = program;

        return CreateMaterial( layout, desc );
    }

    void MaterialManager::Update()
    {
        for ( const auto &material : m_Materials )
        {
            if ( !m_IDManager.IsHandleActive( material->GetID() ) )
                continue;

            auto dirtyFlags = material->GetDirtyFlags();
            if ( dirtyFlags.Has( Material::DirtyFlags::Parameter ) )
            {
                UpdateConstantBuffer( material );
            }

            if ( dirtyFlags.Has( Material::DirtyFlags::Texture ) )
            {
                UpdateBindingSet( material );
            }

            if ( dirtyFlags.HasAny( { Material::DirtyFlags::Parameter, Material::DirtyFlags::Texture } ) )
            {
                dirtyFlags.ClearAll();
            }
        }
    }

    void MaterialManager::UpdateConstantBuffer( Material::Ref material )
    {
        const auto &layout = material->GetLayout();
        const auto &desc = material->GetDescriptor();
        auto &data = m_MaterialData[material->GetID().GetIndex()];

        for ( const auto &param : layout.Parameters )
        {
            const auto &value = desc.Parameters.GetItemAtKey( param.Name );
            const auto &valueData = std::get< primitive::Vector< Byte > >( value );

            SM_ASSERT( param.Size == valueData.GetItemCount() );

            memory::CopyArrayItems( reinterpret_cast< Byte * >( data.ConstantBuffer->GetBuffer() ) + param.Offset,
                param.Size,
                valueData.GetData() );
        }

        // TODO: Upload data to GPU
    }

    void MaterialManager::UpdateBindingSet( Material::Ref material )
    {
        const auto &layout = material->GetLayout();
        const auto &desc = material->GetDescriptor();

        auto &data = m_MaterialData[material->GetID().GetIndex()];

        rhi::BindingSetDescriptor bindingSetDesc{
            { rhi::BindingSetElement::CreateConstantBuffer( layout.CbSlot, data.ConstantBuffer->GetHandle() ) } };

        for ( const auto &textureBinding : layout.Textures )
        {
            Texture::Ref texture = desc.TextureBindings.GetItemAtKey( textureBinding.Name );

            bindingSetDesc.AddItem( rhi::BindingSetElement::CreateTextureSRV(
                textureBinding.Slot, texture->GetHandle(), texture->GetFormat() ) );
        }

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

        data.Bindings = resourceManager.CreateBindingSet( bindingSetDesc, layout.Visibility );
    }
}