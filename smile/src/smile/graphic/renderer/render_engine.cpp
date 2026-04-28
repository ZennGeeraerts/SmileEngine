/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_engine.h"

#include "smile/core/window/window.h"

namespace smile::graphic
{
    memory::Scope< RenderEngine > RenderEngine::Create( rhi::RendererBackendType api )
    {
        auto engine = memory::CreateScope< RenderEngine >( api );

        engine->Initialize();

        return engine;
    }

    RenderEngine::RenderEngine( rhi::RendererBackendType api ) noexcept : m_API{ api }
    {
    }

    void RenderEngine::Initialize() noexcept
    {
        m_RenderContext.Initialize( m_API );

        m_ResourceManager = memory::CreateScope< ResourceManager >( *m_RenderContext.GetGraphicsDevice() );

        m_TextureManager = memory::CreateScope< TextureManager >( GetResourceManager() );
        m_MaterialManager = memory::CreateScope< MaterialManager >( GetTextureManager(), m_ShaderLibrary );

        m_MaterialSystem = memory::CreateScope< MaterialSystem >( m_RenderContext, GetResourceManager() );

        m_ShaderLibrary.LoadShader( "resources/shaders/debug_renderer.vs.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/pos_col.ps.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/pos_tex.vs.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/col_tex.ps.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/skybox.vs.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/skybox.ps.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/pbr.vs.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/pbr.ps.smshader" );
        m_ShaderLibrary.LoadShader( "resources/shaders/pbr_skinned.vs.smshader" );
    }

    void RenderEngine::ShutDown() noexcept
    {
    }

    rhi::SwapChain *RenderEngine::CreateSwapChain( const window::Window *window )
    {
        auto device = m_RenderContext.GetGraphicsDevice();
        auto swapChain = device->CreateSwapChain( window );

        rhi::Object nativeRenderTarget = swapChain->GetNativeRenderTarget();

        const rhi::ObjectType objectType = [&]()
        {
            switch ( m_API )
            {
                case rhi::RendererBackendType::D3D11:
                    return rhi::ObjectType::D3D11_Resource;
                default:
                    SM_ASSERT( false );
            }
        }();

        rhi::TextureDescriptor colorDesc;
        colorDesc.Dimension = rhi::TextureDimension::Texture2D;
        colorDesc.TextureFormat = rhi::Format::RGBA8_UNORM;
        colorDesc.Width = window->GetWidth();
        colorDesc.Height = window->GetHeight();
        colorDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };

        const Texture colorTexture =
            m_ResourceManager->CreateTextureFromNative( nativeRenderTarget, objectType, colorDesc );

        FramebufferAttachment depthAttachment =
            m_ResourceManager->CreateDepthAttachment( window->GetWidth(), window->GetHeight() );

        auto renderTarget = m_ResourceManager->CreateFramebuffer(
            { FramebufferAttachment{ colorTexture, colorDesc.TextureFormat, false } }, depthAttachment );

        auto swapChainPtr = swapChain.get();

        m_SwapChains.PushBack( std::move( swapChain ) );
        m_RenderTargets.Insert( swapChainPtr, std::move( renderTarget ) );

        return swapChainPtr;
    }

    Renderer *RenderEngine::CreateRenderer()
    {
        auto renderer = memory::CreateScope< Renderer >( m_RenderContext, GetResourceManager() );
        m_Renderers.PushBack( std::move( renderer ) );

        return m_Renderers.GetLastItem().GetPointer();
    }

    const Framebuffer &RenderEngine::GetRenderTarget( rhi::SwapChain *const swapChain ) const
    {
        return m_RenderTargets.GetItemAtKey( swapChain );
    }
}