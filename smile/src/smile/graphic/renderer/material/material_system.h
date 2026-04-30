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
#include "material_asset.h"

namespace smile::graphic
{
    class MaterialSystem final
    {
      public:
        MaterialSystem( RenderContext &context, ResourceManager &resourceManager ) noexcept
            : m_Internal{ context, resourceManager }
        {
        }

        Material
        CreateMaterial( const primitive::String &name, const MaterialLayout &layout, const MaterialDescriptor &desc )
        {
            const auto handle = m_Internal.CreateMaterial( name, layout, desc );

            return { handle, &m_Internal };
        }

        Material CreateMaterial( MaterialAsset::ConstRef asset )
        {
            const auto handle =
                m_Internal.CreateMaterial( asset->GetName(), asset->GetLayout(), asset->GetDescriptor() );

            return { handle, &m_Internal };
        }

        void DestroyMaterial( Material &material )
        {
            m_Internal.DestroyMaterial( material.m_Handle );
            material.m_Handle = detail::MaterialHandle::NullHandle();
        }

        MaterialInstance CreateMaterialInstance( const Material material, const MaterialDescriptor &desc )
        {
            const auto handle = m_Internal.CreateMaterialInstance( material.m_Handle, desc );

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
        detail::MaterialSystem m_Internal;
    };
}