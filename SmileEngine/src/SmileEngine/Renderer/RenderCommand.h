#pragma once

#include "SmileEngine/Core.h"
#include "RendererAPI.h"

namespace Smile
{
	class SMILE_API RenderCommand final
	{
	public:
		inline static void CleanUp()
		{
			SAFE_DELETE(m_pRendererAPI);
		}

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

