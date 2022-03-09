#include "smpch.h"
#include "Renderer.h"

namespace smile
{
    RendererSettings Renderer::s_Settings{};
    RenderCollector *Renderer::s_pRenderCollector = new RenderCollector{};

    ShaderLibrary Renderer::s_ShaderLibrary{};

    Ref< Framebuffer > Renderer::s_pFinalSceneFramebuffer{};

    Ref< RasterizerState > Renderer::s_pWireframeRasterizerState{};

    void Renderer::Initialize()
    {
        RenderCommand::Initalize();

        DirectX::XMStoreFloat4x4( &s_pRenderCollector->m_ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->m_ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

        BufferLayout bufferLayout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float3, "NORMAL" } };
        s_ShaderLibrary.Load( "Resources/Shaders/PosColNorm.fx", bufferLayout );

        {
            FramebufferData fbData{};
            fbData.m_Attachments = { { FramebufferTextureFormat::RGBA8, true },
                FramebufferTextureFormat::Depth,
                { FramebufferTextureFormat::RGBA8, true } };
            fbData.m_Width = s_Settings.m_Width;
            fbData.m_Height = s_Settings.m_Height;
            fbData.m_bSwapChainTarget = false;

            s_pFinalSceneFramebuffer = Framebuffer::Create( fbData );
            s_pFinalSceneFramebuffer->SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
                DirectX::Colors::DodgerBlue.f[1],
                DirectX::Colors::DodgerBlue.f[2],
                DirectX::Colors::DodgerBlue.f[3] } );
        }
        {
            RasterizerStateData rasterizerStateData{};
            rasterizerStateData.m_CullMode = CullMode::None;
            rasterizerStateData.m_FillMode = FillMode::WireFrame;
            rasterizerStateData.m_bDepthClipEnable = true;

            s_pWireframeRasterizerState = RasterizerState::Create( rasterizerStateData );
        }
    }

    void Renderer::ShutDown()
    {
        ClearDrawlist();
        SAFE_DELETE( s_pRenderCollector );

        RenderCommand::ShutDown();
    }

    void Renderer::SetSettings( const RendererSettings &settings )
    {
        s_Settings = settings;
    }

    void Renderer::OnWindowResize( uint32_t width, uint32_t height )
    {
        RenderCommand::ResizeWindow( 0, 0, width, height );
    }

    void Renderer::ResizeFramebuffer( uint32_t width, uint32_t height )
    {
        s_Settings.m_Width = width;
        s_Settings.m_Height = height;

        s_pFinalSceneFramebuffer->Resize( width, height );
    }

    void Renderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto ViewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = ViewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &s_pRenderCollector->m_ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->m_ViewInverseMatrix, cameraTransformMat );
    }

    void Renderer::BeginScene( const EditorCamera &editorCamera )
    {
        s_pRenderCollector->m_ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

        DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &viewMatrix );
        DirectX::XMStoreFloat4x4(
            &s_pRenderCollector->m_ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );
    }

    void Renderer::Submit( const Ref< VertexBuffer > &pVertexBuffer,
        const Ref< IndexBuffer > &pIndexBuffer,
        const Ref< Shader > &pShader,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        s_pRenderCollector->m_GeometryDrawList.emplace_back(
            DrawCommand{ pVertexBuffer, pIndexBuffer, pShader, worldTransform } );
    }

    void Renderer::Submit( const MeshRendererComponent &meshRendererComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        Submit( meshRendererComponent.m_pVertexBuffer,
            meshRendererComponent.m_pIndexBuffer,
            meshRendererComponent.m_pShader,
            worldTransform );
    }

    void Renderer::Submit( const StaticMeshComponent &staticMeshComponent, const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &pMesh : staticMeshComponent.m_pMeshes )
        {
            Submit( pMesh->GetVertexBuffer(),
                pMesh->GetIndexBuffer(),
                staticMeshComponent.m_pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void Renderer::Submit( const SkinnedMeshComponent &skinnedMeshComponent, const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &pMesh : skinnedMeshComponent.m_pMeshes )
        {
            Submit( pMesh->GetVertexBuffer(),
                pMesh->GetIndexBuffer(),
                skinnedMeshComponent.m_pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void Renderer::SubmitWireframe( const BoxColliderComponent &boxColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DirectX::XMMATRIX finalTransformMat = DirectX::XMLoadFloat4x4( &worldTransform );
        DirectX::XMVECTOR translationVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR scaleVec{};

        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &translationVec, finalTransformMat );
        DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3( &boxColliderComponent.m_Offset );
        auto finalTranslationVec = DirectX::XMVectorAdd( translationVec, offsetVec );
        DirectX::XMVECTOR sizeVec = DirectX::XMLoadFloat3( &boxColliderComponent.m_Size );

        sizeVec = DirectX::XMVectorDivide( sizeVec, DirectX::XMVECTOR{ 2, 2, 2 } );

        auto finalScaleVec = DirectX::XMVectorMultiply( scaleVec, sizeVec );
        finalTransformMat = DirectX::XMMatrixScalingFromVector( finalScaleVec ) *
                            DirectX::XMMatrixRotationQuaternion( rotationVec ) *
                            DirectX::XMMatrixTranslationFromVector( finalTranslationVec );

        DirectX::XMFLOAT4X4 finalTransform{};
        DirectX::XMStoreFloat4x4( &finalTransform, finalTransformMat );

        DrawCommand drawCommand{ boxColliderComponent.m_pWireframeMesh->GetVertexBuffer(),
            boxColliderComponent.m_pWireframeMesh->GetIndexBuffer(),
            s_ShaderLibrary.Get( "PosColNorm" ),
            finalTransform };
        s_pRenderCollector->m_WireframeDrawList.emplace_back( drawCommand );
    }

    void Renderer::OnRender()
    {
        s_pFinalSceneFramebuffer->Clear();
        s_pFinalSceneFramebuffer->Bind();

        for ( const DrawCommand &drawCommand : s_pRenderCollector->m_GeometryDrawList )
        {
            drawCommand.m_pVertexBuffer->Bind();
            drawCommand.m_pIndexBuffer->Bind();
            drawCommand.m_pShader->Bind();

            drawCommand.m_pShader->UploadMat4( "ViewProjection", s_pRenderCollector->m_ViewProjectionMatrix );
            drawCommand.m_pShader->UploadMat4( "World", drawCommand.m_WorldTransform );
            drawCommand.m_pShader->UploadMat4( "ViewInverse", s_pRenderCollector->m_ViewInverseMatrix );

            RenderCommand::DrawIndexed( drawCommand.m_pIndexBuffer->GetCount(), drawCommand.m_pShader );
        }

        s_pWireframeRasterizerState->Bind();

        for ( const DrawCommand &drawCommand : s_pRenderCollector->m_WireframeDrawList )
        {
            drawCommand.m_pVertexBuffer->Bind();
            drawCommand.m_pIndexBuffer->Bind();
            drawCommand.m_pShader->Bind();

            drawCommand.m_pShader->UploadMat4( "ViewProjection", s_pRenderCollector->m_ViewProjectionMatrix );
            drawCommand.m_pShader->UploadMat4( "World", drawCommand.m_WorldTransform );
            // drawCommand.pShader->UploadMat4("ViewInverse", m_pRenderCollector->ViewInverseMatrix);

            RenderCommand::DrawIndexed( drawCommand.m_pIndexBuffer->GetCount(), drawCommand.m_pShader );
        }

        s_pWireframeRasterizerState->Unbind();

        s_pFinalSceneFramebuffer->Unbind();
    }

    void Renderer::EndScene()
    {
        ClearDrawlist();
    }

    void Renderer::ClearDrawlist()
    {
        s_pRenderCollector->m_GeometryDrawList.clear();
        s_pRenderCollector->m_WireframeDrawList.clear();
    }
}