/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "skybox_renderer.h"

#include "render_engine.h"
#include "engine/graphic/mesh/mesh_factory.h"

namespace smile::graphic
{
    Ref< Mesh > SkyboxRenderer::s_pCubeMesh = nullptr;
    memory::Ref< Shader > SkyboxRenderer::s_pSkyboxShader = nullptr;

    void SkyboxRenderer::Initialize()
    {
        auto pDevice = RenderEngine::GetRenderSystem().GetGraphicsDevice();

        memory::Ref< Texture > pCubeTexture = pDevice->CreateTextureCube( "resources/textures/SkyBox.dds" );
        s_pSkyboxShader = RenderEngine::GetShaderLibrary().Get( "Skybox" );

        s_pSkyboxShader->UploadTexture( "CubeMap", pCubeTexture );

        DirectX::XMFLOAT4X4 worldMatrix;
        DirectX::XMStoreFloat4x4( &worldMatrix, DirectX::XMMatrixIdentity() );
        s_pSkyboxShader->UploadMat4( "World", worldMatrix );

        s_pCubeMesh = MeshFactory::CreateCube( { { ShaderDataType::Float3, "POSITION" } } );
    }

    void SkyboxRenderer::ShutDown()
    {
    }

    void SkyboxRenderer::BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        DirectX::XMVECTOR cameraPosVec;
        DirectX::XMVECTOR cameraRotVec;
        DirectX::XMVECTOR cameraScaleVec;
        DirectX::XMMatrixDecompose(
            &cameraScaleVec, &cameraRotVec, &cameraPosVec, DirectX::XMLoadFloat4x4( &cameraTransform ) );
        DirectX::XMFLOAT3 cameraPos;
        DirectX::XMStoreFloat3( &cameraPos, cameraPosVec );
        DirectX::XMFLOAT4X4 worldMatrix;
        DirectX::XMStoreFloat4x4( &worldMatrix, DirectX::XMMatrixTranslation( cameraPos.x, cameraPos.y, cameraPos.z ) );

        auto projectionMatrixMat = DirectX::XMLoadFloat4x4( &camera.GetProjectionMatrix() );
        auto viewMatrixMat = DirectX::XMMatrixInverse( nullptr, DirectX::XMLoadFloat4x4( &cameraTransform ) );
        auto viewProjectionMatrixMat = viewMatrixMat * projectionMatrixMat;

        DirectX::XMFLOAT4X4 viewProjectionMatrix;
        DirectX::XMStoreFloat4x4( &viewProjectionMatrix, viewProjectionMatrixMat );

        s_pSkyboxShader->UploadMat4( "World", worldMatrix );
        s_pSkyboxShader->UploadMat4( "ViewProjection", viewProjectionMatrix );
    }

    void SkyboxRenderer::OnRender()
    {
        auto pContext = RenderEngine::GetRenderSystem().GetGraphicsContext();

        pContext->BindPrimitiveTopology( PrimitiveTopology::TriangleList );

        pContext->BindShader( s_pSkyboxShader );

        pContext->BindVertexBuffer( s_pCubeMesh->pVertexBuffer );
        pContext->BindIndexBuffer( s_pCubeMesh->pIndexBuffer );

        pContext->DrawIndexed( s_pCubeMesh->pIndexBuffer->Count, s_pSkyboxShader );
    }

    void SkyboxRenderer::EndScene()
    {
    }
}