#pragma once

namespace Smile
{
	enum class CullMode
	{
		eNone,
		eFront, 
		eBack
	};

	enum class FillMode
	{
		eWireFrame,
		eSolid
	};

	struct RasterizerStateData final
	{
		CullMode CullMode = CullMode::eNone;
		FillMode FillMode = FillMode::eSolid;
		bool bDepthClipEnable = true;
	};

	class RasterizerState
	{
	public:
		virtual ~RasterizerState() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<RasterizerState> Create(const RasterizerStateData& rasterizerStateData);
	};
}