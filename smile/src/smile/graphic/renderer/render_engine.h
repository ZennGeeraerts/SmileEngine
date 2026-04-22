/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/scope.h"

#include "render_context.h"
#include "resource/resource_manager.h"
#include "smile/graphic/shader/shader_library.h"
#include "sprite/texture_manager.h"
#include "material/material_system.h"
#include "renderer.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class RenderEngine final
    {
      public:
        static memory::Scope< RenderEngine > Create( rhi::RendererBackendType api );

        void Initialize() noexcept;
        void ShutDown() noexcept;

        [[nodiscard]] rhi::SwapChain *CreateSwapChain( const window::Window *window ) noexcept;
        [[nodiscard]] Renderer *CreateRenderer() noexcept;

        [[nodiscard]] RenderContext &GetRenderContext() noexcept
        {
            return m_RenderContext;
        }

        [[nodiscard]] ResourceManager &GetResourceManager() noexcept
        {
            return m_ResourceManager;
        }

        [[nodiscard]] ShaderLibrary &GetShaderLibrary() noexcept
        {
            return m_ShaderLibrary;
        }

        [[nodiscard]] TextureManager &GetTextureManager() noexcept
        {
            return m_TextureManager;
        }

        [[nodiscard]] MaterialSystem &GetMaterialSystem() noexcept
        {
            return m_MaterialSystem;
        }

        [[nodiscard]] Framebuffer::Ref GetRenderTarget( rhi::SwapChain *const swapChain ) const noexcept;

      public:
        RenderEngine( rhi::RendererBackendType api ) noexcept;

      private:
        rhi::RendererBackendType m_API;
        RenderContext m_RenderContext;
        ResourceManager m_ResourceManager;

        ShaderLibrary m_ShaderLibrary;
        TextureManager m_TextureManager;
        MaterialSystem m_MaterialSystem;

        primitive::Vector< Scope< rhi::SwapChain > > m_SwapChains;
        primitive::Vector< memory::Scope< Renderer > > m_Renderers;

        primitive::HashMap< rhi::SwapChain *, Framebuffer::Ref > m_RenderTargets;
    };
}