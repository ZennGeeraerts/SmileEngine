/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class Sampler final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< Sampler >;
        using ConstRef = memory::Ref< const Sampler >;

        Sampler( rhi::SamplerHandle handle, const rhi::SamplerDescriptor &desc )
            : m_Handle{ handle }, m_Descriptor{ desc }
        {
        }

        rhi::SamplerHandle GetHandle() const
        {
            return m_Handle;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::SamplerHandle m_Handle;
        rhi::SamplerDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}