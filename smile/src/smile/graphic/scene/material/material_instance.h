/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "material_asset.h"
#include "smile/graphic/renderer/resource/texture.h"
#include "smile/graphic/renderer/resource/sampler.h"
#include "smile/core/asset/asset.h"
#include "smile/common/memory/ref.h"

#include <variant>

namespace smile::graphic
{
    enum class MaterialParamType
    {
        Texture,
        Sampler,
        Float,
        Int,
        Bool,
        Float2,
        Float3,
    };

    using MaterialParamValue = std::variant< primitive::Vector< Byte >, Texture::Ref, Sampler::Ref >;

    struct MaterialParam final
    {
        MaterialParam( const primitive::String &name, MaterialParamType type, const MaterialParamValue &value )
            : Name{ name }, Type{ type }, Data{ value }
        {
        }

        primitive::String Name;
        MaterialParamType Type;
        MaterialParamValue Data;
    };

    class MaterialInstance final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< MaterialInstance >;
        using ConstRef = memory::Ref< const MaterialInstance >;

        using ConstantBufferData = primitive::Vector< Byte >;

        MaterialInstance( MaterialAsset::ConstRef pMaterial ) noexcept;

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< MaterialInstance >() };
        }

        void Clear();

        inline const MaterialParam &GetParam( const primitive::StringView name ) const
        {
            return m_Params.GetItemAtKey( name );
        }

        inline const ConstantBufferData &GetConstantBufferData( const primitive::StringView name ) const
        {
            return m_ConstantBufferData.GetItemAtKey( name );
        }

        void SetParam( const primitive::StringView name, const MaterialParamValue &data );

      private:
        MaterialAsset::ConstRef m_pMaterial;
        primitive::HashMap< primitive::String, MaterialParam > m_Params;
        primitive::HashMap< primitive::String, ConstantBufferData > m_ConstantBufferData;
    };
}