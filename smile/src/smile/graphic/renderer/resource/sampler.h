/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/sampler.h"

namespace smile::graphic
{
    class Sampler final
    {
      public:
        Sampler() = default;

        Sampler( rhi::SamplerHandle handle ) noexcept : m_Handle{ handle }
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

        bool operator==( const Sampler &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::SamplerHandle m_Handle;

        friend class ResourceManager;
    };
}