#include "smpch.h"
#include "Renderer.h"
#include "Buffer.h"
namespace Smile
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(RenderingContext* pRenderingContext, const std::shared_ptr<VertexBuffer>& pVertexBuffer, const std::shared_ptr<IndexBuffer>& pIndexBuffer, const std::shared_ptr<Shader>& pShader)
	{
		pVertexBuffer->Bind();
		pIndexBuffer->Bind();
		pShader->Bind();
		RenderCommand::DrawIndexed(pRenderingContext, pIndexBuffer->GetCount(), pShader);
	}
}
