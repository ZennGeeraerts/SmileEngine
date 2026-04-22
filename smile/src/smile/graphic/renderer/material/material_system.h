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

#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"

#include "material.h"
// #include "material_asset.h"

namespace smile::graphic
{
    struct MaterialData final
    {
        Program::ConstRef ShaderProgram;

        ConstantBuffer::Ref ConstantBuffer;
        BindingSet::Ref Bindings;
    };

    class RenderContext;

    class MaterialSystem final
    {
      public:
        using IDManager = primitive::HandleManager< Uint32, 24, 8 >;

        MaterialSystem() = default;
        ~MaterialSystem() = default;

        void Initialize( RenderContext *context ) noexcept;

        Material::Ref
        CreateMaterial( const primitive::String &name, const MaterialLayout &layout, const MaterialDescriptor &desc );
        // Material::Ref CreateMaterial( MaterialAsset::ConstRef asset );

        MaterialInstance::Ref CreateMaterialInstance( Material::ConstRef material, const MaterialDescriptor &desc );

        void UpdateMaterialInstance( MaterialInstance::Ref materialInstance );

        const MaterialData &GetMaterialData( MaterialInstance::ConstRef materialInstance ) const;

      private:
        void UpdateConstantBuffer( MaterialInstance::Ref materialInstance );
        void UpdateBindingSet( MaterialInstance::Ref materialInstance );

        static constexpr Uint16 s_MaxMaterialCount = ( 4 << 10 );
        primitive::Array< MaterialInstance::Ref, s_MaxMaterialCount > m_MaterialInstances;
        primitive::Array< MaterialData, s_MaxMaterialCount > m_MaterialData;
        IDManager m_IDManager;

        RenderContext *m_Context;
    };
}