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
 * @file        material_system.cpp
 * @author      Zenn Geeraerts
 * @created     29 Januari 2026
 * @brief       Manages gpu resources and updates of material
 */
#include "smpch.h"
#include "material_system.h"

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/sprite/texture_manager.h"
#include "smile/common/memory/memory.h"

namespace smile::graphic
{
    Material::Ref MaterialSystem::CreateMaterial( const MaterialLayout &layout, const MaterialDescriptor &desc )
    {
        Material::ID id = m_IDManager.CreateHandle();
        Material::Ref material = memory::CreateRef< Material >( id, layout, desc );

        MaterialData data;
        data.ShaderProgram = desc.ShaderProgram;

        auto &resourceManager = RenderEngine::GetRenderSystem().GetResourceManager();

        const auto &cbDesc = desc.ShaderProgram->GetConstantBufferDescriptor( "Material" );
        data.ConstantBuffer = resourceManager.CreateConstantBuffer( cbDesc );

        return material;
    }

    Material::Ref MaterialSystem::CreateMaterial( MaterialAsset::ConstRef asset )
    {
        return CreateMaterial( asset->GetLayout(), asset->GetDescriptor() );
    }

    void MaterialSystem::Update()
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

    void MaterialSystem::UpdateConstantBuffer( Material::Ref material )
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

    void MaterialSystem::UpdateBindingSet( Material::Ref material )
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

    const MaterialData &MaterialSystem::GetMaterialData( Material::ConstRef material ) const
    {
        const auto id = material->GetID();

        SM_ASSERT( m_IDManager.IsHandleActive( id ) );

        return m_MaterialData[id.GetIndex()];
    }
}