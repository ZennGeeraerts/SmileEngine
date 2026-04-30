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
    class MaterialSystem final : public detail::MaterialSystem
    {
      public:
        MaterialSystem( RenderContext &context, ResourceManager &resourceManager ) noexcept
            : detail::MaterialSystem{ context, resourceManager }
        {
        }

        Material
        CreateMaterial( const primitive::String &name, const MaterialLayout &layout, const MaterialDescriptor &desc )
        {
            const auto handle = detail::MaterialSystem::CreateMaterial( name, layout, desc );

            return { handle, this };
        }

        Material CreateMaterial( MaterialAsset::ConstRef asset )
        {
            const auto handle =
                detail::MaterialSystem::CreateMaterial( asset->GetName(), asset->GetLayout(), asset->GetDescriptor() );

            return { handle, this };
        }

        void DestroyMaterial( Material &material )
        {
            detail::MaterialSystem::DestroyMaterial( material.m_Handle );
            material.m_Handle = detail::MaterialHandle::NullHandle();
        }

        MaterialInstance CreateMaterialInstance( const Material material, const MaterialDescriptor &desc )
        {
            const auto handle = detail::MaterialSystem::CreateMaterialInstance( material.m_Handle, desc );

            return { handle, this };
        }

        void DestroyMaterialInstance( MaterialInstance &instance )
        {
            detail::MaterialSystem::DestroyMaterialInstance( instance.m_Handle );
            instance.m_Handle = detail::MaterialInstanceHandle::NullHandle();
        }

        void UpdateMaterialInstance( const MaterialInstance instance )
        {
            detail::MaterialSystem::UpdateMaterialInstance( instance.m_Handle );
        }

        const MaterialData &GetMaterialData( const MaterialInstance instance ) const
        {
            return detail::MaterialSystem::GetMaterialData( instance.m_Handle );
        }
    };
}