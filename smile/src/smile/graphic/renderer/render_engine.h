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
#include "material/asset/material_manager.h"
#include "material/asset/material_instance_manager.h"
#include "material/material_system.h"
#include "renderer.h"
#include "render_scene.h"

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

        void LoadShaders();

        rhi::SwapChain *CreateSwapChain( const window::Window *window );
        Renderer *CreateRenderer();
        RenderScene *CreateScene( rhi::SwapChain *const swapChain );
        RenderScene *CreateScene( const Framebuffer &framebuffer );

        RenderContext &GetRenderContext() noexcept
        {
            return *m_RenderContext;
        }

        ResourceManager &GetResourceManager() noexcept
        {
            return *m_ResourceManager;
        }

        ShaderLibrary &GetShaderLibrary() noexcept
        {
            return *m_ShaderLibrary;
        }

        TextureManager &GetTextureManager() noexcept
        {
            return *m_TextureManager;
        }

        MaterialManager &GetMaterialManager() noexcept
        {
            return *m_MaterialManager;
        }

        MaterialInstanceManager &GetMaterialInstanceManager() noexcept
        {
            return *m_MaterialInstanceManager;
        }

        MaterialSystem &GetMaterialSystem() noexcept
        {
            return *m_MaterialSystem;
        }

        const Framebuffer &GetRenderTarget( rhi::SwapChain *const swapChain ) const;

      private:
        RenderEngine( rhi::RendererBackendType api,
            memory::Scope< rhi::GraphicsDevice > device,
            memory::Scope< RenderContext > context,
            memory::Scope< ResourceManager > resourceManager,
            memory::Scope< ShaderLibrary > shaderLibrary,
            memory::Scope< TextureManager > textureManager,
            memory::Scope< MaterialManager > materialManager,
            memory::Scope< MaterialInstanceManager > materialInstanceManager,
            memory::Scope< MaterialSystem > materialSystem ) noexcept;

      private:
        rhi::RendererBackendType m_API;
        memory::Scope< rhi::GraphicsDevice > m_Device;
        memory::Scope< RenderContext > m_RenderContext;
        memory::Scope< ResourceManager > m_ResourceManager;

        memory::Scope< ShaderLibrary > m_ShaderLibrary;
        memory::Scope< TextureManager > m_TextureManager;
        memory::Scope< MaterialManager > m_MaterialManager;
        memory::Scope< MaterialInstanceManager > m_MaterialInstanceManager;
        memory::Scope< MaterialSystem > m_MaterialSystem;

        primitive::Vector< memory::Scope< rhi::SwapChain > > m_SwapChains;
        primitive::Vector< memory::Scope< Renderer > > m_Renderers;
        primitive::Vector< memory::Scope< RenderScene > > m_Scenes;

        primitive::HashMap< rhi::SwapChain *, Framebuffer > m_RenderTargets;

        template < typename Type, typename... Args >
        friend constexpr memory::Scope< Type > memory::CreateScope( Args &&... );
    };
}