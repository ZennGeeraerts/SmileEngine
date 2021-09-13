#pragma once

#include "SmileEngine/Core.h"
#include "RendererAPI.h"

namespace Smile
{
	class RenderCommand final
	{
	public:
		inline static void Initalize()
		{
			m_pRendererAPI->Initialize();
		}

		inline static void CleanUp()
		{
			SAFE_DELETE(m_pRendererAPI);
		}

		inline static void SetClearColor(const DirectX::XMFLOAT4& color)
		{
			m_pRendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			m_pRendererAPI->Clear();
		}

		inline static void DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader)
		{
			m_pRendererAPI->DrawIndexed(indexCount, pShader);
		}

	private:
		static RendererAPI* m_pRendererAPI;
	};
}

