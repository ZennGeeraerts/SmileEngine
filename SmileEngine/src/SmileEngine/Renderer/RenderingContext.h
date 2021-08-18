#pragma once

namespace Smile
{
	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;
		virtual void Init() = 0;
		virtual void ClearBackbuffer() = 0;
		virtual void PresentBackbuffer() = 0;
	};
}