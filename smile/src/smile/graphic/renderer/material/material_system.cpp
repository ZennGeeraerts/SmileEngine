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

#include "smile/graphic/renderer/render_context.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/common/memory/memory.h"
#include "smile/common/foundation/traits/type_traits.h"

namespace smile::graphic
{
    MaterialSystem::MaterialSystem( RenderContext &context, ResourceManager &resourceManager ) noexcept
        : m_Context{ context }, m_ResourceManager{ resourceManager }
    {
    }

    Material::Ref MaterialSystem::CreateMaterial( const primitive::String &name,
        const MaterialLayout &layout,
        const MaterialDescriptor &desc )
    {
        Material::Ref material = memory::CreateRef< Material >( name, layout );
        material->m_DefaultInstance = CreateMaterialInstance( material, desc );

        return material;
    }

    /*Material::Ref MaterialSystem::CreateMaterial( MaterialAsset::ConstRef asset )
    {
        return CreateMaterial( asset->GetName(), asset->GetLayout(), asset->GetDescriptor() );
    }*/

    MaterialInstance::Ref MaterialSystem::CreateMaterialInstance( Material::ConstRef material,
        const MaterialDescriptor &desc )
    {
        MaterialInstance::ID id = m_IDManager.CreateHandle();

        MaterialInstance::Ref materialInstance = memory::CreateRef< MaterialInstance >( id, desc, *material );

        MaterialData data;
        data.ShaderProgram = desc.ShaderProgram;

        const auto &cbDesc = desc.ShaderProgram->GetConstantBufferDescriptor( "Material" );
        data.ConstantBuffer = m_ResourceManager.CreateConstantBuffer( cbDesc );

        m_MaterialInstances[id.GetIndex()] = materialInstance;
        m_MaterialData[id.GetIndex()] = std::move( data );

        return materialInstance;
    }

    void MaterialSystem::UpdateMaterialInstance( MaterialInstance::Ref materialInstance )
    {
        auto dirtyFlags = materialInstance->GetDirtyFlags();
        if ( dirtyFlags.Has( MaterialInstance::DirtyFlags::Parameter ) )
        {
            UpdateConstantBuffer( materialInstance );
        }

        if ( dirtyFlags.Has( MaterialInstance::DirtyFlags::Texture ) )
        {
            UpdateBindingSet( materialInstance );
        }

        if ( dirtyFlags.HasAny( { MaterialInstance::DirtyFlags::Parameter, MaterialInstance::DirtyFlags::Texture } ) )
        {
            materialInstance->ClearDirtyFlags();
        }
    }

    void MaterialSystem::UpdateConstantBuffer( MaterialInstance::Ref materialInstance )
    {
        const auto &layout = materialInstance->GetMaterial().GetLayout();
        const auto &desc = materialInstance->GetDescriptor();
        auto &data = m_MaterialData[materialInstance->GetID().GetIndex()];

        primitive::Vector< Byte > bufferData( layout.CbSize );

        for ( const auto &param : layout.Parameters )
        {
            const auto &value = desc.Parameters.GetItemAtKey( param.Name );

            std::visit(
                [&]( const auto &val )
                {
                    using ValueType = std::decay_t< decltype( val ) >;

                    if constexpr ( std::is_same_v< ValueType, primitive::Vector< Byte > > )
                    {
                        SM_ASSERT( param.Size == val.GetItemCount() );

                        memory::CopyArrayItems( bufferData.GetData() + param.Offset, param.Size, val.GetData() );
                    }
                    else if constexpr ( std::is_trivially_copyable_v< ValueType > )
                    {
                        std::memcpy( bufferData.GetData() + param.Offset, &val, param.Size );
                    }
                    else
                    {
                        static_assert( foundation::AlwaysFalseV< ValueType >, "Type not supported" );
                    }
                },
                value );
        }

        data.ConstantBuffer.UpdateBuffer( bufferData.GetData(), bufferData.GetItemCount() );

        m_Context.FillConstantBuffer( data.ConstantBuffer );
    }

    void MaterialSystem::UpdateBindingSet( MaterialInstance::Ref materialInstance )
    {
        const auto &layout = materialInstance->GetMaterial().GetLayout();
        const auto &desc = materialInstance->GetDescriptor();

        auto &data = m_MaterialData[materialInstance->GetID().GetIndex()];

        rhi::BindingSetDescriptor bindingSetDesc{
            { rhi::BindingSetElement::CreateConstantBuffer( layout.CbSlot, data.ConstantBuffer.GetHandle() ) } };

        for ( const auto &textureBinding : layout.Textures )
        {
            const MaterialTextureBinding textureParam = desc.TextureBindings.GetItemAtKey( textureBinding.Name );

            if ( textureParam.Texture.IsValid() )
            {
                bindingSetDesc.AddItem( rhi::BindingSetElement::CreateTextureSRV(
                    textureBinding.Slot, textureParam.Texture.GetHandle(), textureParam.Texture.GetFormat() ) );

                const Sampler sampler = m_ResourceManager.GetOrCreateSampler( textureParam.SamplerDescriptor );

                bindingSetDesc.AddItem(
                    rhi::BindingSetElement::CreateSampler( textureBinding.Slot, sampler.GetHandle() ) );
            }
        }

        m_ResourceManager.CreateBindingSetAndLayout(
            bindingSetDesc, layout.Visibility, data.BindingLayout, data.Bindings );
    }

    const MaterialData &MaterialSystem::GetMaterialData( MaterialInstance::ConstRef materialInstance ) const
    {
        const auto id = materialInstance->GetID();

        SM_ASSERT( m_IDManager.IsHandleActive( id ) );

        return m_MaterialData[id.GetIndex()];
    }
}