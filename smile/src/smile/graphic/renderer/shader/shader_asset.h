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

      private:
        primitive::String m_Name;
        primitive::Vector< Byte > m_ByteCode;
        ShaderReflectionData m_ReflectionData;

        std::variant< VertexShader::Ref, PixelShader::Ref > m_pShader;
    };
}