/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "rhi_test_layer.h"

#include "smile/core/application/application.h"

#include "smile/graphic/rhi/swap_chain.h"
#include "smile/graphic/shader/shader_loader.h"
#include "smile/graphic/shader/shader_asset.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    RHITestLayer::RHITestLayer()
    {
        rhi::RendererBackendType api = rhi::RendererBackendType::DirectX11;
        m_pDevice = rhi::GraphicsDevice::Create( api );
        m_pImmediateCommandList = m_pDevice->CreateCommandList();
    }

    void RHITestLayer::OnAttach()
    {
        auto &window = application::Application::GetInstance().GetMainWindow();
        m_pSwapChain = m_pDevice->CreateSwapChain( &window );

        rhi::Object nativeRenderTarget = m_pSwapChain->GetNativeRenderTarget();

        rhi::TextureDescriptor colorDesc;
        colorDesc.Dimension = rhi::TextureDimension::Texture2D;
        colorDesc.TextureFormat = rhi::Format::RGBA8_UNORM;
        colorDesc.Width = window.GetWidth();
        colorDesc.Height = window.GetHeight();
        colorDesc.BindFlags = { rhi::TextureBindFlags::RenderTarget };

        rhi::TextureHandle swapchainTexture = m_TextureHandleManager.CreateHandle();
        m_pDevice->CreateHandleForNativeTexture(
            swapchainTexture, nativeRenderTarget, rhi::ObjectType::D3D11_Resource, colorDesc );

        m_FramebufferDesc.ColorAttachments.PushBack( { swapchainTexture, colorDesc } );

        m_Framebuffer = m_FramebufferManager.CreateHandle();
        m_pDevice->CreateFramebuffer( m_Framebuffer, m_FramebufferDesc );

        graphic::ShaderLoader shaderLoader{};
        {
            auto pShaderAsset = shaderLoader.LoadShader( "resources/shaders/pos_tex.vs.smshader" );
            const auto &reflectionData = pShaderAsset->GetReflectionData();

            m_VertexShaderHandle = m_ShaderHandleManager.CreateHandle();
            rhi::ShaderDescriptor desc{};
            desc.EntryPoint = reflectionData.EntryPoint;
            desc.TargetProfile = reflectionData.TargetProfile;
            desc.Stage = rhi::ShaderStage::Vertex;
            m_pDevice->CreateShader( m_VertexShaderHandle, desc, pShaderAsset->GetByteCode() );
        }

        {
            auto pShaderAsset = shaderLoader.LoadShader( "resources/shaders/col_tex.ps.smshader" );
            const auto &reflectionData = pShaderAsset->GetReflectionData();

            m_PixelShaderHandle = m_ShaderHandleManager.CreateHandle();
            rhi::ShaderDescriptor desc{};
            desc.EntryPoint = reflectionData.EntryPoint;
            desc.TargetProfile = reflectionData.TargetProfile;
            desc.Stage = rhi::ShaderStage::Pixel;
            m_pDevice->CreateShader( m_PixelShaderHandle, desc, pShaderAsset->GetByteCode() );
        }
    }

    void RHITestLayer::OnDetach()
    {
    }

    void RHITestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        for ( const auto &colorAttachment : m_FramebufferDesc.ColorAttachments )
        {
            m_pImmediateCommandList->ClearTexture( colorAttachment.Texture,
                {},
                math::Color{ DirectX::Colors::CornflowerBlue.f[0],
                    DirectX::Colors::CornflowerBlue.f[1],
                    DirectX::Colors::CornflowerBlue.f[2],
                    DirectX::Colors::CornflowerBlue.f[3] } );
        }

        m_pSwapChain->Present();
    }

    void RHITestLayer::OnEvent( window::Event &event )
    {
    }

    void RHITestLayer::OnImGuiRender()
    {
    }
}