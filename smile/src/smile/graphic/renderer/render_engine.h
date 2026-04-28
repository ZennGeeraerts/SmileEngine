/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/scope.h"

#include "render_context.h"
#include "smile/graphic/rhi/swap_chain.h"
#include "resource/resource_manager.h"
#include "smile/graphic/shader/shader_library.h"
#include "sprite/texture_manager.h"
#include "material/material_manager.h"
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

        rhi::SwapChain *CreateSwapChain( const window::Window *window );
        Renderer *CreateRenderer();

        RenderContext &GetRenderContext() noexcept
        {
            return m_RenderContext;
        }

        ResourceManager &GetResourceManager() noexcept
        {
            return *m_ResourceManager;
        }
        
        ShaderLibrary &GetShaderLibrary() noexcept
        {
            return m_ShaderLibrary;
        }

        TextureManager &GetTextureManager() noexcept
        {
            return *m_TextureManager;
        }

        MaterialManager &GetMaterialManager() noexcept
        {
            return *m_MaterialManager;
        }

        MaterialSystem &GetMaterialSystem() noexcept
        {
            return *m_MaterialSystem;
        }

        const Framebuffer &GetRenderTarget( rhi::SwapChain *const swapChain ) const;

      public:
        RenderEngine( rhi::RendererBackendType api ) noexcept;

      private:
        rhi::RendererBackendType m_API;
        RenderContext m_RenderContext;
        memory::Scope< ResourceManager > m_ResourceManager;

        ShaderLibrary m_ShaderLibrary;
        memory::Scope< TextureManager > m_TextureManager;
        memory::Scope< MaterialManager > m_MaterialManager;
        memory::Scope< MaterialSystem > m_MaterialSystem;

        primitive::Vector< Scope< rhi::SwapChain > > m_SwapChains;
        primitive::Vector< memory::Scope< Renderer > > m_Renderers;

        primitive::HashMap< rhi::SwapChain *, Framebuffer > m_RenderTargets;
    };
}