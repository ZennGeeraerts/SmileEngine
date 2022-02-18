#pragma once

#include "SmileEngine/Core/Core.h"
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

		inline static void ShutDown()
		{
			SAFE_DELETE(m_pRendererAPI);
		}

		inline static void ResizeWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			m_pRendererAPI->ResizeWindow(x, y, width, height);
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

