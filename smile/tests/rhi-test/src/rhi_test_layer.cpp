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
    struct Material final
    {
        DirectX::XMFLOAT3 Color{ 0.8f, 0.5f, 0.0f };
        int UseTexture{ 0 };
    };

    RHITestLayer::RHITestLayer()
    {
        rhi::RendererBackendType api = rhi::RendererBackendType::D3D11;
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

        m_FramebufferInfo = rhi::FramebufferInfoExtented{ m_FramebufferDesc };

        graphic::ShaderLoader shaderLoader{};
        {
            auto pShaderAsset = shaderLoader.Load( "resources/shaders/pos_tex.vs.smshader" );
            const auto &reflectionData = pShaderAsset->GetReflectionData();

            m_VertexShaderHandle = m_ShaderHandleManager.CreateHandle();
            rhi::ShaderDescriptor desc{};
            desc.EntryPoint = reflectionData.EntryPoint;
            desc.TargetProfile = reflectionData.TargetProfile;
            desc.Stage = rhi::ShaderStage::Vertex;
            m_pDevice->CreateShader( m_VertexShaderHandle, desc, pShaderAsset->GetByteCode() );
        }

        {
            auto pShaderAsset = shaderLoader.Load( "resources/shaders/col_tex.ps.smshader" );
            const auto &reflectionData = pShaderAsset->GetReflectionData();

            m_PixelShaderHandle = m_ShaderHandleManager.CreateHandle();
            rhi::ShaderDescriptor desc{};
            desc.EntryPoint = reflectionData.EntryPoint;
            desc.TargetProfile = reflectionData.TargetProfile;
            desc.Stage = rhi::ShaderStage::Pixel;
            m_pDevice->CreateShader( m_PixelShaderHandle, desc, pShaderAsset->GetByteCode() );
        }

        rhi::BufferLayout vertexLayout{
            { rhi::Format::RGB32_FLOAT, "POSITION" }, { rhi::Format::RG32_FLOAT, "TEXCOORD" } };

        rhi::BindingLayout vsBindingLayout{ { rhi::ShaderStage::Vertex } };
        vsBindingLayout.AddElement( { 0, rhi::ResourceType::ConstantBuffer } );
        vsBindingLayout.AddElement( { 1, rhi::ResourceType::ConstantBuffer } );

        rhi::BindingLayoutHandle vsBindingLayoutHandle = m_BindingLayoutHandleManager.CreateHandle();
        m_pDevice->CreateBindingLayout( vsBindingLayoutHandle, vsBindingLayout );

        rhi::BindingLayout psBindingLayout{ { rhi::ShaderStage::Pixel } };
        psBindingLayout.AddElement( { 2, rhi::ResourceType::ConstantBuffer } );
        psBindingLayout.AddElement( { 0, rhi::ResourceType::Texture_SRV } );
        psBindingLayout.AddElement( { 0, rhi::ResourceType::Sampler } );

        rhi::BindingLayoutHandle psBindingLayoutHandle = m_BindingLayoutHandleManager.CreateHandle();
        m_pDevice->CreateBindingLayout( psBindingLayoutHandle, psBindingLayout );

        {
            rhi::GraphicsPipelineDescriptor psoDesc{};
            psoDesc.Topology = rhi::PrimitiveTopology::TriangleList;
            psoDesc.InputLayout = vertexLayout;
            psoDesc.VertexShaderHandle = m_VertexShaderHandle;
            psoDesc.PixelShaderHandle = m_PixelShaderHandle;

            psoDesc.BindingLayouts.PushBack( vsBindingLayoutHandle );
            psoDesc.BindingLayouts.PushBack( psBindingLayoutHandle );

            psoDesc.State.RasterizerState.CullMode = rhi::CullMode::None;
            psoDesc.State.DepthStencilState.DepthEnable = false;

            m_PipelineHandle = m_PipelineHandleManager.CreateHandle();
            m_pDevice->CreateGraphicsPipeline( m_PipelineHandle, psoDesc, m_FramebufferInfo );
        }

        {
            rhi::GPUBufferDescriptor bufferDesc{};
            bufferDesc.Size = GetConstantTypeInfo( ConstantType::Mat4 ).Size;
            bufferDesc.Usage = rhi::BufferUsage::Dynamic;
            bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
            bufferDesc.BindFlags = { rhi::BufferBindFlags::ConstantBuffer };

            m_CameraConstantBufferHandle = m_GPUBufferHandleManager.CreateHandle();
            m_pDevice->CreateGPUBuffer( m_CameraConstantBufferHandle, bufferDesc );
        }

        {
            rhi::GPUBufferDescriptor bufferDesc{};
            bufferDesc.Size = GetConstantTypeInfo( ConstantType::Mat4 ).Size;
            bufferDesc.Usage = rhi::BufferUsage::Dynamic;
            bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
            bufferDesc.BindFlags = { rhi::BufferBindFlags::ConstantBuffer };

            m_PerObjectBufferHandle = m_GPUBufferHandleManager.CreateHandle();
            m_pDevice->CreateGPUBuffer( m_PerObjectBufferHandle, bufferDesc );
        }

        {
            rhi::GPUBufferDescriptor bufferDesc{};
            bufferDesc.Size = sizeof( Material );
            bufferDesc.Usage = rhi::BufferUsage::Dynamic;
            bufferDesc.CPUAccess = rhi::CPUAccessMode::Write;
            bufferDesc.BindFlags = { rhi::BufferBindFlags::ConstantBuffer };

            m_MaterialBufferHandle = m_GPUBufferHandleManager.CreateHandle();
            m_pDevice->CreateGPUBuffer( m_MaterialBufferHandle, bufferDesc );
        }

        {
            rhi::SamplerDescriptor desc{};
            m_SamplerHandle = m_SamplerHandleManager.CreateHandle();
            m_pDevice->CreateSampler( m_SamplerHandle, desc );
        }

        {
            rhi::BindingSetDescriptor vsBindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 0, m_CameraConstantBufferHandle ),
                    rhi::BindingSetElement::CreateConstantBuffer( 1, m_PerObjectBufferHandle ) } };

            rhi::BindingSetDescriptor psBindingSetDesc{
                { rhi::BindingSetElement::CreateConstantBuffer( 2, m_MaterialBufferHandle ),
                    rhi::BindingSetElement::CreateSampler( 0, m_SamplerHandle ) } };

            m_VSBindingSetHandle = m_BindingSetHandleManager.CreateHandle();
            m_pDevice->CreateBindingSet( m_VSBindingSetHandle, vsBindingSetDesc, vsBindingLayoutHandle );

            m_PSBindingSetHandle = m_BindingSetHandleManager.CreateHandle();
            m_pDevice->CreateBindingSet( m_PSBindingSetHandle, psBindingSetDesc, psBindingLayoutHandle );
        }

        {
            const Count vertexCount = 3;
            float vertices[]{
                -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 1.0f };

            rhi::GPUBufferDescriptor bufferDesc{};
            bufferDesc.Size = vertexCount * vertexLayout.GetStride();
            bufferDesc.Usage = rhi::BufferUsage::Immutable;
            bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
            bufferDesc.BindFlags = { rhi::BufferBindFlags::VertexBuffer };

            m_VertexBufferHandle = m_GPUBufferHandleManager.CreateHandle();
            m_pDevice->CreateGPUBuffer( m_VertexBufferHandle, bufferDesc, vertices );
        }

        {
            Uint32 indices[]{ 0, 1, 2 };

            rhi::GPUBufferDescriptor bufferDesc{};
            bufferDesc.Size = 3 * sizeof( Uint32 );
            bufferDesc.Usage = rhi::BufferUsage::Immutable;
            bufferDesc.CPUAccess = rhi::CPUAccessMode::None;
            bufferDesc.BindFlags = { rhi::BufferBindFlags::IndexBuffer };

            m_IndexBufferHandle = m_GPUBufferHandleManager.CreateHandle();
            m_pDevice->CreateGPUBuffer( m_IndexBufferHandle, bufferDesc, indices );
        }
    }

    void RHITestLayer::OnDetach()
    {
    }

    void RHITestLayer::OnUpdate( primitive::Timestep deltaTime )
    {
        m_pImmediateCommandList->Open();

        for ( const auto &colorAttachment : m_FramebufferDesc.ColorAttachments )
        {
            m_pImmediateCommandList->ClearTexture( colorAttachment.Texture,
                {},
                math::Color{ DirectX::Colors::CornflowerBlue.f[0],
                    DirectX::Colors::CornflowerBlue.f[1],
                    DirectX::Colors::CornflowerBlue.f[2],
                    DirectX::Colors::CornflowerBlue.f[3] } );
        }

        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMStoreFloat4x4( &viewProjection, DirectX::XMMatrixIdentity() );
        m_pImmediateCommandList->FillBuffer(
            m_CameraConstantBufferHandle, &viewProjection, sizeof( DirectX::XMFLOAT4X4 ) );
        m_pImmediateCommandList->FillBuffer( m_PerObjectBufferHandle, &viewProjection, sizeof( DirectX::XMFLOAT4X4 ) );

        Material material{};
        m_pImmediateCommandList->FillBuffer( m_MaterialBufferHandle, &material, sizeof( Material ) );

        rhi::GraphicsState state{};
        state.Pipeline = m_PipelineHandle;
        state.Framebuffer = m_Framebuffer;
        state.Viewport.Viewports.PushBack( m_FramebufferInfo.GetViewport( 0.0f, 1.0f ) );
        state.Bindings.PushBack( m_VSBindingSetHandle );
        state.Bindings.PushBack( m_PSBindingSetHandle );
        state.VertexBuffers.PushBack( rhi::VertexBufferBinding{ m_VertexBufferHandle, 0, 0 } );
        state.IndexBuffer = rhi::IndexBufferBinding{ m_IndexBufferHandle, rhi::Format::R32_UINT, 0 };

        m_pImmediateCommandList->SetGraphicsState( state );
        m_pImmediateCommandList->DrawIndexed( { 3, 0, 0 } );

        m_pSwapChain->Present();

        m_pImmediateCommandList->Close();
    }

    void RHITestLayer::OnEvent( window::Event &event )
    {
    }

    void RHITestLayer::OnImGuiRender()
    {
    }
}