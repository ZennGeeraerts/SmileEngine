#pragma once

namespace Smile
{
	enum class RendererAPI
	{
		eNone = 0,
		eDirectX11
	};

	class Renderer final
	{
	public:
		inline static RendererAPI GetAPI() { return m_RenderAPI; }

	private:
		static RendererAPI m_RenderAPI;
	};
}

