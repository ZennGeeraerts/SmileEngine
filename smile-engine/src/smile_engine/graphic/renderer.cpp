#include "smpch.h"
#include "renderer.h"

namespace smile::graphic
{
    RendererSettings Renderer::s_Settings{};
    RenderCollector *Renderer::s_pRenderCollector = new RenderCollector{};

    ShaderLibrary Renderer::s_ShaderLibrary{};

    Ref< Framebuffer > Renderer::s_pFinalSceneFramebuffer{};

    Ref< RasterizerState > Renderer::s_pWireframeRasterizerState{};

    void Renderer::Initialize()
    {
        RenderCommand::Initalize();

        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixIdentity() );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewProjectionMatrix, DirectX::XMMatrixIdentity() );

        BufferLayout bufferLayout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float3, "NORMAL" } };
        s_ShaderLibrary.Load( "assets/shaders/PosColNorm.fx", bufferLayout );
        s_ShaderLibrary.Load( "assets/shaders/PosCol.fx", { { ShaderDataType::Float3, "POSITION" } } );

        {
            FramebufferDescriptor frameBufferDesc{};
            frameBufferDesc.Attachments = { { FramebufferTextureFormat::RGBA8, true },
                FramebufferTextureFormat::Depth,
                { FramebufferTextureFormat::RGBA8, true } };
            frameBufferDesc.Width = s_Settings.Width;
            frameBufferDesc.Height = s_Settings.Height;
            frameBufferDesc.IsSwapChainTarget = false;

            s_pFinalSceneFramebuffer = Framebuffer::Create( frameBufferDesc );
            s_pFinalSceneFramebuffer->SetClearColor( { DirectX::Colors::DodgerBlue.f[0],
                DirectX::Colors::DodgerBlue.f[1],
                DirectX::Colors::DodgerBlue.f[2],
                DirectX::Colors::DodgerBlue.f[3] } );
        }
        {
            RasterizerStateDescriptor rasterizerStateDesc{};
            rasterizerStateDesc.CullMode = CullMode::None;
            rasterizerStateDesc.FillMode = FillMode::WireFrame;
            rasterizerStateDesc.EnableDepthClip = true;

            s_pWireframeRasterizerState = RasterizerState::Create( rasterizerStateDesc );
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

    void Renderer::OnWindowResize( Uint32 width, Uint32 height )
    {
        RenderCommand::ResizeWindow( 0, 0, width, height );
    }

    void Renderer::ResizeFramebuffer( Uint32 width, Uint32 height )
    {
        s_Settings.Width = width;
        s_Settings.Height = height;

        s_pFinalSceneFramebuffer->Resize( width, height );
    }

    void Renderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto cameraTransformMat = DirectX::XMLoadFloat4x4( &cameraTransform );
        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, cameraTransformMat );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewProjectionMatrix, viewProjectionMatrixMat );
        DirectX::XMStoreFloat4x4( &s_pRenderCollector->ViewInverseMatrix, cameraTransformMat );
    }

    void Renderer::BeginScene( const EditorCamera &editorCamera )
    {
        s_pRenderCollector->ViewProjectionMatrix = editorCamera.GetViewProjectionMatrix();

        DirectX::XMFLOAT4X4 viewMatrix = editorCamera.GetViewMatrix();
        auto viewMatrixMat = DirectX::XMLoadFloat4x4( &viewMatrix );
        DirectX::XMStoreFloat4x4(
            &s_pRenderCollector->ViewInverseMatrix, DirectX::XMMatrixInverse( nullptr, viewMatrixMat ) );
    }

    void Renderer::Submit( const Ref< VertexBuffer > &pVertexBuffer,
        const Ref< IndexBuffer > &pIndexBuffer,
        const Ref< Shader > &pShader,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        s_pRenderCollector->GeometryDrawList.emplace_back(
            DrawCommand{ pVertexBuffer, pIndexBuffer, pShader, worldTransform } );
    }

    void Renderer::Submit( const scene::MeshRendererComponent &meshRendererComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        Submit( meshRendererComponent.pVertexBuffer,
            meshRendererComponent.pIndexBuffer,
            meshRendererComponent.pShader,
            worldTransform );
    }

    void Renderer::Submit( const scene::StaticMeshComponent &staticMeshComponent, const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &mesh : staticMeshComponent.pMeshes )
        {
            Submit( mesh->GetVertexBuffer(),
                mesh->GetIndexBuffer(),
                staticMeshComponent.pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void Renderer::Submit( const scene::SkinnedMeshComponent &skinnedMeshComponent, const DirectX::XMFLOAT4X4 &worldTransform )
    {
        for ( const auto &mesh : skinnedMeshComponent.pMeshes )
        {
            Submit( mesh->GetVertexBuffer(),
                mesh->GetIndexBuffer(),
                skinnedMeshComponent.pMaterials[0]->GetShader(),
                worldTransform );
        }
    }

    void Renderer::SubmitWireframe( const scene::BoxColliderComponent &boxColliderComponent,
        const DirectX::XMFLOAT4X4 &worldTransform )
    {
        DirectX::XMMATRIX finalTransformMat = DirectX::XMLoadFloat4x4( &worldTransform );
        DirectX::XMVECTOR translationVec{};
        DirectX::XMVECTOR rotationVec{};
        DirectX::XMVECTOR scaleVec{};

        DirectX::XMMatrixDecompose( &scaleVec, &rotationVec, &translationVec, finalTransformMat );
        DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3( &boxColliderComponent.Offset );
        auto finalTranslationVec = DirectX::XMVectorAdd( translationVec, offsetVec );
        DirectX::XMVECTOR sizeVec = DirectX::XMLoadFloat3( &boxColliderComponent.Size );

        sizeVec = DirectX::XMVectorDivide( sizeVec, DirectX::XMVECTOR{ 2, 2, 2 } );

        auto finalScaleVec = DirectX::XMVectorMultiply( scaleVec, sizeVec );
        finalTransformMat = DirectX::XMMatrixScalingFromVector( finalScaleVec ) *
                            DirectX::XMMatrixRotationQuaternion( rotationVec ) *
                            DirectX::XMMatrixTranslationFromVector( finalTranslationVec );

        DirectX::XMFLOAT4X4 finalTransform{};
        DirectX::XMStoreFloat4x4( &finalTransform, finalTransformMat );

        DrawCommand drawCommand{ boxColliderComponent.pWireframeMesh->GetVertexBuffer(),
            boxColliderComponent.pWireframeMesh->GetIndexBuffer(),
            s_ShaderLibrary.Get( "PosCol" ),
            finalTransform };
        s_pRenderCollector->WireframeDrawList.emplace_back( drawCommand );
    }

    void Renderer::OnRender()
    {
        s_pFinalSceneFramebuffer->Clear();
        s_pFinalSceneFramebuffer->Bind();

        for ( const DrawCommand &drawCommand : s_pRenderCollector->GeometryDrawList )
        {
            drawCommand.pVertexBuffer->Bind();
            drawCommand.pIndexBuffer->Bind();
            drawCommand.pShader->Bind();

            drawCommand.pShader->UploadMat4( "ViewProjection", s_pRenderCollector->ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            drawCommand.pShader->UploadMat4( "ViewInverse", s_pRenderCollector->ViewInverseMatrix );

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->GetCount(), drawCommand.pShader );
        }

        s_pWireframeRasterizerState->Bind();

        for ( const DrawCommand &drawCommand : s_pRenderCollector->WireframeDrawList )
        {
            drawCommand.pVertexBuffer->Bind();
            drawCommand.pIndexBuffer->Bind();
            drawCommand.pShader->Bind();

            drawCommand.pShader->UploadMat4( "ViewProjection", s_pRenderCollector->ViewProjectionMatrix );
            drawCommand.pShader->UploadMat4( "World", drawCommand.WorldTransform );
            // drawCommand.pShader->UploadMat4("ViewInverse", m_pRenderCollector->ViewInverseMatrix);

            RenderCommand::DrawIndexed( drawCommand.pIndexBuffer->GetCount(), drawCommand.pShader );
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
        s_pRenderCollector->GeometryDrawList.clear();
        s_pRenderCollector->WireframeDrawList.clear();
    }
}