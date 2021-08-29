#pragma once

#include "RenderCommand.h"
#include "Buffer.h"
#include "Shader.h"

namespace Smile
{
	class Renderer final
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(RenderingContext* pRenderingContext, const std::shared_ptr<VertexBuffer>& pVertexBuffer, const std::shared_ptr<IndexBuffer>& pIndexBuffer, const std::shared_ptr<Shader>& pShader);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

