#pragma once
#include "SmileEngine/Renderer/RenderingContext.h"
#include "SmileRasterDeviceContext.cuh"

namespace Smile
{
	class Window;
	class SmileRasterDeviceContext;

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

		SmileRasterDeviceContext* GetDeviceContext() const { return m_pDeviceContext; }

	private:
		Window* m_pWindow = nullptr;

		SmileRasterDeviceContext* m_pDeviceContext = nullptr;
		HDC m_HDC = nullptr;
		HBITMAP m_Bitmap = nullptr;
		HBITMAP m_BitmapOld = nullptr;
		uint8_t* m_pScreenBuffer{};

		friend class SmileRasterRendererAPI;
	};
}

