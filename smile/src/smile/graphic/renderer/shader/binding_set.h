/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/shader/binding_set.h"

namespace smile::graphic
{
    class BindingSet final
    {
      public:
        BindingSet() = default;

        BindingSet( rhi::BindingSetHandle handle ) noexcept : m_Handle{ handle }
        {
        }

        BindingSet( const BindingSet & ) = default;
        BindingSet( BindingSet && ) noexcept = default;
        ~BindingSet() = default;

        BindingSet &operator=( const BindingSet & ) = default;
        BindingSet &operator=( BindingSet && ) noexcept = default;

        rhi::BindingSetHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        bool operator==( const BindingSet &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::BindingSetHandle m_Handle;

        friend class ResourceManager;
    };
}