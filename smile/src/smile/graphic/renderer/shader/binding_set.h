/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/shader/binding_set.h"
#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic
{
    class BindingSet final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< BindingSet >;

        BindingSet( rhi::BindingSetHandle handle,
            const rhi::BindingSetDescriptor &descriptor,
            const rhi::BindingLayout &layout )
            : m_Handle{ handle }, m_Descriptor{ descriptor }, m_Layout{ layout }
        {
        }

        rhi::BindingSetHandle GetHandle() const
        {
            return m_Handle;
        }

        bool IsValid() const
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