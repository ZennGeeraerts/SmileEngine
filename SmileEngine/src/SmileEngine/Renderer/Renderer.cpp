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

	void Renderer::Submit(RenderingContext* pRenderingContext, const std::shared_ptr<VertexBuffer>& pVertexBuffer, const std::shared_ptr<IndexBuffer>& pIndexBuffer, const std::shared_ptr<Shader>& pShader)
	{
		pVertexBuffer->Bind();
		pIndexBuffer->Bind();
		pShader->Bind();
		pShader->UploadMat4("WorldViewProjection", m_pSceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(pRenderingContext, pIndexBuffer->GetCount(), pShader);
	}
}
