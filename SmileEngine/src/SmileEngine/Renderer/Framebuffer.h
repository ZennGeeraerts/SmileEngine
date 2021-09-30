#pragma once
#include "SmileEngine/Core/Core.h"

namespace Smile
{
	struct FramebufferData
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;

		// if true -> Render to the swapchain
		bool bSwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Invalidate() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) = 0;
		virtual void Clear() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FramebufferData& GetData() const = 0;
		virtual void* GetColor() const = 0;

		static Ref<Framebuffer> Create(const FramebufferData& framebufferData);
	};
}

