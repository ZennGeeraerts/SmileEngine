#pragma once

#include "SmileEngine/Renderer/RendererAPI.h"

namespace Smile
{
	class DirectX11RendererAPI final : public RendererAPI
	{
	public:
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader) override;

	private:
		DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
	};
}

