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
 * @file        material_asset.h
 * @author      Zenn Geeraerts
 * @created     9 Januari 2026
 * @brief       Asset for material
 */
#pragma once

#include "smile/core/asset/asset.h"
#include "smile/graphic/shader/shader_asset.h"
#include "smile/graphic/shader/constant_buffer_descriptor.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    class MaterialAsset final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< MaterialAsset >;
        using ConstRef = memory::Ref< const MaterialAsset >;

        MaterialAsset( ShaderAsset::ConstRef pVertexShader, ShaderAsset::ConstRef pPixelShader ) noexcept;

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialAsset >() };
        }

        void SetShaders( const ShaderAsset::ConstRef &pVertexShader, const ShaderAsset::ConstRef &pPixelShader );

        inline const primitive::HashMap< primitive::String, rhi::BindingLayoutElement > &GetBindings() const
        {
            return m_Bindings;
        }

        inline const ConstantBufferDescriptor &GetConstantBufferDesc( const primitive::StringView name ) const
        {
            return m_ConstantBufferDescs.GetItemAtKey( name );
        }

        inline const primitive::HashMap< primitive::String, ConstantBufferDescriptor > &GetConstantBufferDescs() const
        {
            return m_ConstantBufferDescs;
        }

        ShaderAsset::ConstRef GetVertexShader() const
        {
            return m_pVertexShader;
        }

        ShaderAsset::ConstRef GetPixelShader() const
        {
            return m_pPixelShader;
        }

      private:
        ShaderAsset::ConstRef m_pVertexShader;
        ShaderAsset::ConstRef m_pPixelShader;

        primitive::HashMap< primitive::String, rhi::BindingLayoutElement > m_Bindings;
        primitive::HashMap< primitive::String, ConstantBufferDescriptor > m_ConstantBufferDescs;
    };
}