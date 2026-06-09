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
 * @file        material.h
 * @author      Zenn Geeraerts
 * @created     30 April 2026
 * @brief       Public facing API, object oriented material / instance class
 */
#pragma once

#include "detail/material_handle.h"
#include "material_layout.h"
#include "material_descriptor.h"

namespace smile::graphic
{
    namespace detail
    {
        class MaterialSystem;
    }

    struct MaterialData;
    class MaterialInstance;

    class Material final
    {
      public:
        Material() = default;
        Material( const detail::MaterialHandle handle, detail::MaterialSystem *system ) noexcept;

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );
        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            const Texture &texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        [[nodiscard]] bool IsValid() const noexcept;

        MaterialInstance GetDefaultInstance() const;

        const MaterialLayout &GetLayout() const;

      private:
        detail::MaterialHandle m_Handle;
        detail::MaterialSystem *m_System = nullptr;

        friend class MaterialSystem;
    };

    class MaterialInstance final
    {
      public:
        MaterialInstance() = default;
        MaterialInstance( const detail::MaterialInstanceHandle handle, detail::MaterialSystem *system ) noexcept;

        void SetParameter( const primitive::StringView name, const MaterialParameterValue &value );

        const MaterialParameterValue &GetParameter( const primitive::StringView name ) const;

        void SetTextureBinding( const primitive::StringView name,
            const Texture &texture,
            const rhi::SamplerDescriptor &samplerDesc );

        const MaterialTextureBinding &GetTextureBinding( const primitive::StringView name ) const;

        [[nodiscard]] bool IsValid() const noexcept;

        Material GetMaterial() const;

        detail::MaterialInstanceHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hashCode = m_Handle.Hash();
            hashCode = foundation::HashCombine( hashCode, std::hash< detail::MaterialSystem * >{}( m_System ) );
            return hashCode;
        }

        bool operator==( const MaterialInstance &other ) const noexcept
        {
            return m_Handle == other.m_Handle && m_System == other.m_System;
        }

        bool operator!=( const MaterialInstance &other ) const noexcept
        {
            return !( *this == other );
        }

      private:
        detail::MaterialInstanceHandle m_Handle;
        detail::MaterialSystem *m_System = nullptr;

        friend class MaterialSystem;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::MaterialInstance >
    {
        smile::foundation::HashCode operator()( const smile::graphic::MaterialInstance &instance ) const noexcept
        {
            return instance.GetHashCode();
        }
    };
}