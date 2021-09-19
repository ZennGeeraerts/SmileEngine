#pragma once

#include "SmileEngine/Renderer/RendererAPI.h"
#include "DirectX11Context.h"

namespace Smile
{
	class DirectX11RendererAPI final : public RendererAPI
	{
	public:
		virtual void Initialize() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader) override;

	private:
		DirectX::XMFLOAT4 m_ClearColor = { 1.f, 1.f, 1.f, 1.f };
		DirectX11Context* m_pDirectX11Context = nullptr;
	};
}

