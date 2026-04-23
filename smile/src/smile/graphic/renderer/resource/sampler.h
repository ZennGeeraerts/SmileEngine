/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/sampler.h"

namespace smile::graphic
{
    class Sampler final
    {
      public:
        Sampler() = default;

        Sampler( rhi::SamplerHandle handle, const rhi::SamplerDescriptor &desc ) noexcept
            : m_Handle{ handle }, m_Descriptor{ desc }
        {
        }

        Sampler( const Sampler & ) = default;
        Sampler( Sampler && ) noexcept = default;

        ~Sampler() = default;

        Sampler &operator=( const Sampler & ) = default;
        Sampler &operator=( Sampler && ) noexcept = default;

        rhi::SamplerHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::SamplerHandle m_Handle;
        rhi::SamplerDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}