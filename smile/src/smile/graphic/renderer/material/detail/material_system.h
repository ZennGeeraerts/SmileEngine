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
 * @created     29 Januari 2026
 * @brief       Manages gpu resources and updates of material
 */
#pragma once

#include "smile/common/primitive/handle_manager.h"
#include "smile/common/primitive/collection/array.h"

#include "smile/graphic/renderer/shader/binding_layout.h"
#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"

#include "material.h"
#include "material_instance.h"

namespace smile::graphic
{
    class RenderContext;
    class ResourceManager;

    struct MaterialData final
    {
        Program::ConstRef ShaderProgram;

        ConstantBuffer ConstantBuffer;
        BindingLayout BindingLayout;
        BindingSet Bindings;
    };

    namespace detail
    {
        class MaterialSystem
        {
          public:
            MaterialSystem( RenderContext &context, ResourceManager &resourceManager ) noexcept;
            virtual ~MaterialSystem();

            Material &GetMaterial( const MaterialHandle handle );
            const Material &GetMaterial( const MaterialHandle handle ) const;

            MaterialInstance &GetMaterialInstance( const MaterialInstanceHandle handle );
            const MaterialInstance &GetMaterialInstance( const MaterialInstanceHandle handle ) const;

          protected:
            MaterialHandle CreateMaterial( const primitive::String &name,
                const MaterialLayout &layout,
                const MaterialDescriptor &desc );

            void DestroyMaterial( const MaterialHandle handle );

            MaterialInstanceHandle CreateMaterialInstance( const MaterialHandle materialHandle,
                const MaterialDescriptor &desc );

            void DestroyMaterialInstance( const MaterialInstanceHandle handle );

            void UpdateMaterialInstance( const MaterialInstanceHandle handle );

            const MaterialData &GetMaterialData( const MaterialInstanceHandle handle ) const;

          private:
            void UpdateConstantBuffer( const MaterialInstance &materialInstance );
            void UpdateBindingSet( const MaterialInstance &materialInstance );

            [[nodiscard]] bool IsMaterialValid( const MaterialHandle handle ) const noexcept
            {
                return handle.IsValid() && m_MaterialHandleManager.IsHandleActive( handle );
            }

            [[nodiscard]] bool IsMaterialInstanceValid( const MaterialInstanceHandle handle ) const noexcept
            {
                return handle.IsValid() && m_MaterialInstanceHandleManager.IsHandleActive( handle );
            }

            static constexpr Uint16 s_MaxMaterialCount = ( 4 << 10 );

            primitive::Array< Material, s_MaxMaterialCount > m_Materials;
            primitive::Array< MaterialInstance, s_MaxMaterialCount > m_MaterialInstances;
            primitive::Array< MaterialData, s_MaxMaterialCount > m_MaterialData;

            MaterialHandleManager m_MaterialHandleManager;
            MaterialInstanceHandleManager m_MaterialInstanceHandleManager;

            RenderContext &m_Context;
            ResourceManager &m_ResourceManager;
        };
    }
}