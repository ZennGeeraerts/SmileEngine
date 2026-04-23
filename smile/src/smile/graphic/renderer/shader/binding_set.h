/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/shader/binding_set.h"
#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic
{
    class BindingSet final
    {
      public:
        BindingSet() = default;

        BindingSet( rhi::BindingSetHandle handle,
            const rhi::BindingSetDescriptor &descriptor,
            const rhi::BindingLayout &layout ) noexcept
            : m_Handle{ handle }, m_Descriptor{ descriptor }, m_Layout{ layout }
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

        const rhi::BindingLayout &GetLayout() const noexcept
        {
            return m_Layout;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::BindingSetHandle m_Handle;
        rhi::BindingSetDescriptor m_Descriptor;
        rhi::BindingLayout m_Layout;

        friend class ResourceManager;
    };
}