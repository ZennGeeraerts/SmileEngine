/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/shader/shader_reflection_data.h"

#include <optional>

namespace smile::graphic
{
    class VertexShader final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< VertexShader >;
        using ConstRef = memory::Ref< const VertexShader >;

        VertexShader( rhi::ShaderHandle handle, std::optional< ShaderReflectionData > reflectionData = std::nullopt )
            : m_Handle{ handle }, m_ReflectionData{ std::move( reflectionData ) }
        {
        }

        ~VertexShader() = default;

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

        const ShaderReflectionData *GetReflectionData() const
        {
            return m_ReflectionData.has_value() ? &m_ReflectionData.value() : nullptr;
        }

      private:
        rhi::ShaderHandle m_Handle;
        std::optional< ShaderReflectionData > m_ReflectionData;

        friend class ResourceManager;
    };
}