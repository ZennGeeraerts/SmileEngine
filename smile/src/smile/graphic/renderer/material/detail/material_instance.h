/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "material_handle.h"
#include "smile/graphic/renderer/material/material_descriptor.h"

#include "smile/common/foundation/flags.h"

namespace smile::graphic::detail
{
    class MaterialInstance final
    {
      public:
        using Handle = MaterialInstanceHandle;

        enum class DirtyFlags
        {
            Parameter,
            Texture,
        };

        MaterialInstance() = default;

        MaterialInstance( const MaterialInstanceHandle handle,
            const MaterialDescriptor &desc,
            const MaterialHandle materialHandle ) noexcept;

        ~MaterialInstance() = default;

        void Clear() noexcept;

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            const Texture &texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        Handle GetHandle() const noexcept
        {
            return m_Handle;
        }

        const MaterialDescriptor &GetDescriptor() const noexcept
        {
            return m_Descriptor;
        }

        MaterialHandle GetMaterialHandle() const noexcept
        {
            return m_MaterialHandle;
        }

        foundation::Flags< DirtyFlags > GetDirtyFlags() const noexcept
        {
            return m_DirtyFlags;
        }

        constexpr void ClearDirtyFlags() noexcept
        {
            m_DirtyFlags.ClearAll();
        }

      private:
        Handle m_Handle;
        MaterialDescriptor m_Descriptor;
        MaterialHandle m_MaterialHandle;
        foundation::Flags< DirtyFlags > m_DirtyFlags;
    };
}