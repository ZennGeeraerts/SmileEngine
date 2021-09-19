#pragma once

#include "RenderingContext.h"
#include "Shader.h"

namespace Smile
{
	class RendererAPI
	{
	public:
		enum class API
		{
			eNone = 0,
			eDirectX11 = 1
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Initialize() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader) = 0;

		inline static API GetAPI() { return m_API; }

	private:
		static API m_API;
	};
}

