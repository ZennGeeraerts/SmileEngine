#pragma once

#include "RendererAPI.h"

namespace Smile
{
	class RenderCommand final
	{
	public:
		inline static void SetClearColor(const DirectX::XMFLOAT4& color)
		{
			m_pRendererAPI->SetClearColor(color);
		}

		inline static void Clear(RenderingContext* pRenderingContext)
		{
			m_pRendererAPI->Clear(pRenderingContext);
		}

		inline static void DrawIndexed(RenderingContext* pRenderingContext, int32_t indexCount, const std::shared_ptr<Shader>& pShader)
		{
			m_pRendererAPI->DrawIndexed(pRenderingContext, indexCount, pShader);
		}

	private:
		static RendererAPI* m_pRendererAPI;
	};
}

