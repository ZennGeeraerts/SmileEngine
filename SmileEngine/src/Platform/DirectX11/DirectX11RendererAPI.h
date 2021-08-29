#pragma once

#include "SmileEngine/Renderer/RendererAPI.h"

namespace Smile
{
	class DirectX11RendererAPI final : public RendererAPI
	{
	public:
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear(RenderingContext* pRenderingContext) override;

		virtual void DrawIndexed(RenderingContext* pRenderingContext, int32_t indexCount, const std::shared_ptr<Shader>& pShader) override;

	private:
		DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
	};
}

