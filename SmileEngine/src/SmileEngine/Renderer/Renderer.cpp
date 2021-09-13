#include "smpch.h"
#include "Renderer.h"

namespace Smile
{
	Renderer::SceneData* Renderer::m_pSceneData = new Renderer::SceneData{};

	void Renderer::Initialize()
	{
		RenderCommand::Initalize();
	}

	void Renderer::CleanUp()
	{
		SAFE_DELETE(m_pSceneData);
		RenderCommand::CleanUp();
	}

	void Renderer::BeginScene(const Camera& camera, const DirectX::XMFLOAT4X4& cameraTransform)
	{
		auto cameraTransformMat = DirectX::XMLoadFloat4x4(&cameraTransform);
		auto projectionMatrixMat = DirectX::XMLoadFloat4x4(&camera.GetProjectionMatrix());
		auto ViewMatrixMat = DirectX::XMMatrixInverse(nullptr, cameraTransformMat);
		auto viewProjectionMatrixMat = ViewMatrixMat * projectionMatrixMat;

		DirectX::XMStoreFloat4x4(&m_pSceneData->ViewProjectionMatrix, viewProjectionMatrixMat);
		DirectX::XMStoreFloat4x4(&m_pSceneData->ViewInverseMatrix, cameraTransformMat);
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexBuffer>& pVertexBuffer, const Ref<IndexBuffer>& pIndexBuffer, const Ref<Shader>& pShader, 
		const DirectX::XMFLOAT4X4& worldTransform)
	{
		pVertexBuffer->Bind();
		pIndexBuffer->Bind();
		pShader->Bind();

		pShader->UploadMat4("ViewProjection", m_pSceneData->ViewProjectionMatrix);
		pShader->UploadMat4("World", worldTransform);
		pShader->UploadMat4("ViewInverse", m_pSceneData->ViewInverseMatrix);
		RenderCommand::DrawIndexed(pIndexBuffer->GetCount(), pShader);
	}

	void Renderer::Submit(const MeshRendererComponent& meshRendererComponent, const DirectX::XMFLOAT4X4& worldTransform)
	{
		Submit(meshRendererComponent.pVertexBuffer, meshRendererComponent.pIndexBuffer, meshRendererComponent.pShader, worldTransform);
	}

	void Renderer::Submit(const StaticMeshComponent& staticMeshComponent, const DirectX::XMFLOAT4X4& worldTransform)
	{
		for (const auto& pMesh : staticMeshComponent.m_pMeshes)
		{
			Submit(pMesh->GetVertexBuffer(), pMesh->GetIndexBuffer(), pMesh->GetShader(), worldTransform);
		}
	}
}
