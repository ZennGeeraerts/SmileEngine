#pragma once

namespace Smile
{
	enum class FramebufferTextureFormat
	{
		eNone = 0,
		eRGBA8,
		eDEPTH24STENCIL8,

		eDepth = eDEPTH24STENCIL8
	};

	struct FramebufferTextureData final
	{
		FramebufferTextureData() = default;
		FramebufferTextureData(FramebufferTextureFormat format, bool bUseShader = false)
			: TextureFormat{ format }
			, bUseInShader{ bUseShader }
		{}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::eNone;
		bool bUseInShader;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentData final
	{
		FramebufferAttachmentData() = default;
		FramebufferAttachmentData(const std::initializer_list<FramebufferTextureData>& attachments)
			: Attachments{ attachments }
		{}

		std::vector<FramebufferTextureData> Attachments;
	};

	struct FramebufferData final
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		FramebufferAttachmentData Attachments;
		uint32_t Samples = 1;

		// if true -> Render to the swapchain
		bool bSwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) = 0;
		virtual void Clear() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const FramebufferData& GetData() const = 0;
		virtual void* GetColor(uint32_t index) const = 0;

		static Ref<Framebuffer> Create(const FramebufferData& framebufferData);
	};
}

