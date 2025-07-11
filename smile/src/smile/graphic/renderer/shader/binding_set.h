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

        BindingSet( BindingSetHandle handle, const BindingSetDescriptor &descriptor, const BindingLayout &layout )
            : m_Handle{ handle }, m_Descriptor{ descriptor }, m_Layout{ layout }
        {
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        BindingSetHandle m_Handle;
        BindingSetDescriptor m_Descriptor;
        BindingLayout m_Layout;

        friend class ResourceManager;
    };
}