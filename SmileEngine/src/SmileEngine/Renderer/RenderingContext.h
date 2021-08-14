#pragma once

namespace Smile
{
	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	private:

	};
}