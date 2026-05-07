/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/memory/ref.h"
#include "smile/core/asset/asset.h"
#include "shader_reflection_data.h"

namespace smile::graphic
{
    class ShaderAsset final : public asset::Asset< ShaderAsset >
    {
      public:
        using Ref = memory::Ref< ShaderAsset >;
        using ConstRef = memory::Ref< const ShaderAsset >;

        ShaderAsset() = default;

        ShaderAsset( const primitive::String &name,
            const primitive::Vector< Byte > &byteCode,
            const ShaderReflectionData &reflectionData )
            : m_Name{ name }, m_ByteCode{ byteCode }, m_ReflectionData{ reflectionData }
        {
        }

        primitive::StringView GetName() const
        {
            return m_Name.AsStringView();
        }

        const ShaderReflectionData &GetReflectionData() const
        {
            return m_ReflectionData;
        }

        const primitive::Vector< Byte > &GetByteCode() const
        {
            return m_ByteCode;
        }

      private:
        primitive::String m_Name;
        primitive::Vector< Byte > m_ByteCode;
        ShaderReflectionData m_ReflectionData;

        friend class ShaderSerializer;
    };
}