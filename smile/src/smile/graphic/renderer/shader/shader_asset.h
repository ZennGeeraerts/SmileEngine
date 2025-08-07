/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/core/asset/asset.h"
#include "shader_reflection_data.h"
#include "vertex_shader.h"
#include "pixel_shader.h"

#include <variant>

namespace smile::graphic
{
    class ShaderAsset final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< ShaderAsset >;
        using ConstRef = memory::Ref< const ShaderAsset >;

        ShaderAsset( const primitive::String &name,
            const primitive::Vector< Byte > &byteCode,
            const ShaderReflectionData &reflectionData )
            : m_Name{ name }, m_ByteCode{ byteCode }, m_ReflectionData{ reflectionData }
        {
        }

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< ShaderAsset >() };
        }

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

        const ShaderReflectionData &GetReflectionData() const
        {
            return m_ReflectionData;
        }

        VertexShader::Ref GetVertexShader() const
        {
            if ( auto ppVertexShader = std::get_if< VertexShader::Ref >( &m_pShader ) )
            {
                return *ppVertexShader;
            }

            return nullptr;
        }

        PixelShader::Ref GetPixelShader() const
        {
            if ( auto ppPixelShader = std::get_if< PixelShader::Ref >( &m_pShader ) )
            {
                return *ppPixelShader;
            }

            return nullptr;
        }

      private:
        primitive::String m_Name;
        primitive::Vector< Byte > m_ByteCode;
        ShaderReflectionData m_ReflectionData;

        std::variant< VertexShader::Ref, PixelShader::Ref > m_pShader;
    };
}