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

namespace smile::graphic::detail
{
    MaterialSystem::MaterialSystem( RenderContext &context, ResourceManager &resourceManager ) noexcept
        : m_Context{ context }, m_ResourceManager{ resourceManager }
    {
    }

    MaterialSystem::~MaterialSystem()
    {
        for ( const auto materialHandle : m_MaterialHandleManager )
        {
            DestroyMaterial( materialHandle );
        }

        for ( const auto instanceHandle : m_MaterialInstanceHandleManager )
        {
            DestroyMaterialInstance( instanceHandle );
        }
    }

    detail::MaterialHandle MaterialSystem::CreateMaterial( const primitive::String &name,
        const MaterialLayout &layout,
        const MaterialDescriptor &desc )
    {
        auto handle = m_MaterialHandleManager.CreateHandle();

        const auto instanceHandle = CreateMaterialInstance( handle, desc );
        auto &material = m_Materials[handle.GetIndex()];
        material = detail::Material{ name, layout, instanceHandle };

        return handle;
    }

    void MaterialSystem::DestroyMaterial( const detail::MaterialHandle handle )
    {
        const auto &material = GetMaterial( handle );

        DestroyMaterialInstance( material.GetDefaultInstance() );

        m_MaterialHandleManager.DestroyHandle( handle );
    }

    MaterialInstanceHandle MaterialSystem::CreateMaterialInstance( const MaterialHandle materialHandle,
        const MaterialDescriptor &desc )
    {
        const auto handle = m_MaterialInstanceHandleManager.CreateHandle();
        const auto index = handle.GetIndex();

        auto &materialInstance = m_MaterialInstances[index];
        materialInstance = detail::MaterialInstance{ handle, desc, materialHandle };

        MaterialData data;
        data.ShaderProgram = desc.ShaderProgram;

        const auto &cbDesc = desc.ShaderProgram->GetConstantBufferDescriptor( "Material" );
        data.ConstantBuffer = m_ResourceManager.CreateConstantBuffer( cbDesc );

        m_MaterialData[index] = std::move( data );

        return handle;
    }

    void MaterialSystem::DestroyMaterialInstance( const detail::MaterialInstanceHandle handle )
    {
        SM_ASSERT( IsMaterialInstanceValid( handle ) );

        auto &materialData = m_MaterialData[handle.GetIndex()];

        m_ResourceManager.DestroyConstantBuffer( materialData.ConstantBuffer );
        m_ResourceManager.DestroyBindingLayout( materialData.BindingLayout );
        m_ResourceManager.DestroyBindingSet( materialData.Bindings );

        m_MaterialInstanceHandleManager.DestroyHandle( handle );
    }

    void MaterialSystem::UpdateMaterialInstance( const MaterialInstanceHandle handle )
    {
        auto &materialInstance = m_MaterialInstances[handle.GetIndex()];

        auto dirtyFlags = materialInstance.GetDirtyFlags();
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
            materialInstance.ClearDirtyFlags();
        }
    }

    void MaterialSystem::UpdateConstantBuffer( const MaterialInstance &materialInstance )
    {
        const auto &material = GetMaterial( materialInstance.GetMaterialHandle() );
        const auto &layout = material.GetLayout();
        const auto &desc = materialInstance.GetDescriptor();

        auto &data = m_MaterialData[materialInstance.GetHandle().GetIndex()];

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

    void MaterialSystem::UpdateBindingSet( const MaterialInstance &materialInstance )
    {
        const auto &material = GetMaterial( materialInstance.GetMaterialHandle() );
        const auto &layout = material.GetLayout();
        const auto &desc = materialInstance.GetDescriptor();

        auto &data = m_MaterialData[materialInstance.GetHandle().GetIndex()];

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

        if ( data.BindingLayout.IsValid() )
            m_ResourceManager.DestroyBindingLayout( data.BindingLayout );

        if ( data.Bindings.IsValid() )
            m_ResourceManager.DestroyBindingSet( data.Bindings );

        m_ResourceManager.CreateBindingSetAndLayout(
            bindingSetDesc, layout.Visibility, data.BindingLayout, data.Bindings );
    }

    const MaterialData &MaterialSystem::GetMaterialData( const MaterialInstanceHandle handle ) const
    {
        SM_ASSERT( IsMaterialInstanceValid( handle ) );

        return m_MaterialData[handle.GetIndex()];
    }

    Material &MaterialSystem::GetMaterial( const MaterialHandle handle )
    {
        SM_ASSERT( IsMaterialValid( handle ) );

        return m_Materials[handle.GetIndex()];
    }

    const Material &MaterialSystem::GetMaterial( const MaterialHandle handle ) const
    {
        SM_ASSERT( IsMaterialValid( handle ) );

        return m_Materials[handle.GetIndex()];
    }

    MaterialInstance &MaterialSystem::GetMaterialInstance( const MaterialInstanceHandle handle )
    {
        SM_ASSERT( IsMaterialInstanceValid( handle ) );

        return m_MaterialInstances[handle.GetIndex()];
    }

    const MaterialInstance &MaterialSystem::GetMaterialInstance( const MaterialInstanceHandle handle ) const
    {
        SM_ASSERT( IsMaterialInstanceValid( handle ) );

        return m_MaterialInstances[handle.GetIndex()];
    }

    MaterialInstance &MaterialSystem::GetDefaultInstance( const MaterialHandle handle )
    {
        const auto instanceHandle = GetMaterial( handle ).GetDefaultInstance();

        return GetMaterialInstance( instanceHandle );
    }

    const MaterialInstance &MaterialSystem::GetDefaultInstance( const MaterialHandle handle ) const
    {
        const auto instanceHandle = GetMaterial( handle ).GetDefaultInstance();

        return GetMaterialInstance( instanceHandle );
    }

    void MaterialSystem::SetMaterialParameter( const MaterialHandle handle,
        const primitive::StringView name,
        const MaterialParameterValue &value )
    {
        GetDefaultInstance( handle ).SetParameter( name, value );
    }

    const MaterialParameterValue &MaterialSystem::GetMaterialParameter( const MaterialHandle handle,
        const primitive::StringView name ) const
    {
        return GetDefaultInstance( handle ).GetParameter( name );
    }

    void MaterialSystem::SetMaterialTextureBinding( const MaterialHandle handle,
        const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        GetDefaultInstance( handle ).SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &MaterialSystem::GetMaterialTextureBinding( const MaterialHandle handle,
        const primitive::StringView name ) const
    {
        return GetDefaultInstance( handle ).GetTextureBinding( name );
    }

    void MaterialSystem::SetMaterialInstanceParameter( const MaterialInstanceHandle handle,
        const primitive::StringView name,
        const MaterialParameterValue &value )
    {
        GetMaterialInstance( handle ).SetParameter( name, value );
    }

    const MaterialParameterValue &MaterialSystem::GetMaterialInstanceParameter( const MaterialInstanceHandle handle,
        const primitive::StringView name ) const
    {
        return GetMaterialInstance( handle ).GetParameter( name );
    }

    void MaterialSystem::SetMaterialInstanceTextureBinding( const MaterialInstanceHandle handle,
        const primitive::StringView name,
        const Texture &texture,
        const rhi::SamplerDescriptor &samplerDesc )
    {
        GetMaterialInstance( handle ).SetTextureBinding( name, texture, samplerDesc );
    }

    const MaterialTextureBinding &MaterialSystem::GetMaterialInstanceTextureBinding(
        const MaterialInstanceHandle handle,
        const primitive::StringView name ) const
    {
        return GetMaterialInstance( handle ).GetTextureBinding( name );
    }

    MaterialInstanceHandle MaterialSystem::GetDefaultInstanceHandle( const MaterialHandle handle ) const
    {
        return GetMaterial( handle ).GetDefaultInstance();
    }

    const MaterialLayout &MaterialSystem::GetMaterialLayout( const MaterialHandle handle ) const
    {
        return GetMaterial( handle ).GetLayout();
    }

    MaterialHandle MaterialSystem::GetMaterialHandle( const MaterialInstanceHandle instanceHandle ) const
    {
        return GetMaterialInstance( instanceHandle ).GetMaterialHandle();
    }
}