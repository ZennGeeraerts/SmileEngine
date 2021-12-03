#pragma once
#include "SmileEngine/Renderer/RenderingContext.h"

namespace Smile
{
	class Window;

	class SmileRasterContext final : public RenderingContext
	{
	public:
		SmileRasterContext(Window* pWindow);
		virtual ~SmileRasterContext();

		SmileRasterContext(const SmileRasterContext&) = delete;
		SmileRasterContext(SmileRasterContext&&) = delete;
		SmileRasterContext& operator=(const SmileRasterContext&) = delete;
		SmileRasterContext& operator=(SmileRasterContext&&) = delete;

		virtual void Init() override;
		virtual void Present() override;

	private:
		Window* m_pWindow = nullptr;

		HDC m_DeviceContext = nullptr;
		HBITMAP m_Bitmap = nullptr;
		HBITMAP m_BitmapOld = nullptr;
		uint8_t* m_pFrontBuffer = nullptr;

		friend class SmileRasterRendererAPI;
	};
}

