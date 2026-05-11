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
 * @file        material_system.h
 * @author      Zenn Geeraerts
 * @created     30 April 2026
 * @brief       Public material system API
 */
#pragma once

#include "detail/material_system.h"
#include "material.h"
#include "asset/material_asset.h"
#include "asset/material_instance_asset.h"

namespace smile::graphic
{
    class TextureManager;

    class MaterialSystem final
    {
      public:
        MaterialSystem( RenderContext &context,
            ResourceManager &resourceManager,
            TextureManager &textureManager ) noexcept;

        Material
        CreateMaterial( const primitive::String &name, const MaterialLayout &layout, const MaterialDescriptor &desc )
        {
            const auto handle = m_Internal.CreateMaterial( name, layout, desc );

            return { handle, &m_Internal };
        }

        Material CreateMaterial( MaterialAsset::ConstRef asset )
        {
            SM_ASSERT( !m_AssetToMaterial.HasItemAtKey( asset.GetPointer() ) );

            const auto &layout = asset->GetLayout();

            const auto handle = m_Internal.CreateMaterial(
                asset->GetName(), layout, BuildMaterialDescriptor( asset->GetDescriptor(), layout ) );

            m_AssetToMaterial.Insert( asset.GetPointer(), handle );

            return { handle, &m_Internal };
        }

        Material GetOrCreateMaterial( MaterialAsset::ConstRef asset )
        {
            if ( m_AssetToMaterial.HasItemAtKey( asset.GetPointer() ) )
            {
                const auto handle = m_AssetToMaterial[asset.GetPointer()];
                return { handle, &m_Internal };
            }

            return CreateMaterial( asset );
        }

        void DestroyMaterial( Material &material )
        {
            for ( auto it = m_AssetToMaterial.begin(); it != m_AssetToMaterial.end(); ++it )
            {
                if ( it.GetItem() == material.m_Handle )
                {
                    m_AssetToMaterial.Erase( it );
                    break;
                }
            }

            m_Internal.DestroyMaterial( material.m_Handle );
            material.m_Handle = detail::MaterialHandle::NullHandle();
        }

        MaterialInstance CreateMaterialInstance( const Material material, const MaterialDescriptor &desc )
        {
            const auto handle = m_Internal.CreateMaterialInstance( material.m_Handle, desc );

            return { handle, &m_Internal };
        }

        MaterialInstance CreateMaterialInstance( MaterialInstanceAsset::ConstRef asset )
        {
            auto material = GetOrCreateMaterial( asset->GetMaterialAsset() );

            const auto &layout = m_Internal.GetMaterialLayout( material.m_Handle );

            const auto handle = m_Internal.CreateMaterialInstance(
                material.m_Handle, BuildMaterialDescriptor( asset->GetDescriptor(), layout ) );

            return { handle, &m_Internal };
        }

        void DestroyMaterialInstance( MaterialInstance &instance )
        {
            m_Internal.DestroyMaterialInstance( instance.m_Handle );
            instance.m_Handle = detail::MaterialInstanceHandle::NullHandle();
        }

        void UpdateMaterialInstance( const MaterialInstance instance )
        {
            m_Internal.UpdateMaterialInstance( instance.m_Handle );
        }

        const MaterialData &GetMaterialData( const MaterialInstance instance ) const
        {
            return m_Internal.GetMaterialData( instance.m_Handle );
        }

      private:
        MaterialDescriptor BuildMaterialDescriptor( const MaterialAssetDescriptor &assetDesc,
            const MaterialLayout &layout );

        detail::MaterialSystem m_Internal;
        primitive::HashMap< const MaterialAsset *, detail::MaterialHandle > m_AssetToMaterial;
        TextureManager &m_TextureManager;
    };
}