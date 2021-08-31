#include "smpch.h"
#include "Renderer.h"

namespace Smile
{
	Renderer::SceneData* Renderer::m_pSceneData = new Renderer::SceneData{};

	void Renderer::CleanUp()
	{
		SAFE_DELETE(m_pSceneData);
		RenderCommand::CleanUp();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_pSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(RenderingContext* pRenderingContext, const std::shared_ptr<VertexBuffer>& pVertexBuffer, const std::shared_ptr<IndexBuffer>& pIndexBuffer, const std::shared_ptr<Shader>& pShader, 
		const DirectX::XMFLOAT4X4& worldTransform)
	{
		pVertexBuffer->Bind();
		pIndexBuffer->Bind();
		pShader->Bind();

		auto worldViewProjectionMatrixMat = DirectX::XMLoadFloat4x4(&worldTransform) * DirectX::XMLoadFloat4x4(&m_pSceneData->ViewProjectionMatrix);
		DirectX::XMFLOAT4X4 worldViewProjectionMatrix{};
		DirectX::XMStoreFloat4x4(&worldViewProjectionMatrix, worldViewProjectionMatrixMat);

		pShader->UploadMat4("WorldViewProjection", worldViewProjectionMatrix);
		pShader->UploadMat4("World", worldTransform);
		RenderCommand::DrawIndexed(pRenderingContext, pIndexBuffer->GetCount(), pShader);
	}
}
