#include "smpch.h"
#include "SmileRasterRendererAPI.h"

#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	void SmileRasterRendererAPI::Initialize()
	{
		m_pWindow = &SmileGame::GetInstance().GetWindow();
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(m_pWindow->GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterRendererAPI > RenderingContext is not a SmileRasterContext");
	}

	void SmileRasterRendererAPI::ResizeWindow(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	void SmileRasterRendererAPI::SetClearColor(const DirectX::XMFLOAT4& color)
	{
		m_ClearColor = color;
	}

	void SmileRasterRendererAPI::Clear()
	{
		uint32_t width = m_pWindow->GetWidth();
		uint32_t height = m_pWindow->GetHeight();

		for (uint32_t y{}; y < height; ++y)
		{
			for (uint32_t x{}; x < width; ++x)
			{
				uint32_t currentPixel = (y * width + x) * 3;

				m_pSmileRasterContext->m_pFrontBuffer[currentPixel] = static_cast<uint8_t>(m_ClearColor.z * 255.f);
				m_pSmileRasterContext->m_pFrontBuffer[currentPixel + 1] = static_cast<uint8_t>(m_ClearColor.y * 255.f);
				m_pSmileRasterContext->m_pFrontBuffer[currentPixel + 2] = static_cast<uint8_t>(m_ClearColor.x * 255.f);
			}
		}
	}

	void SmileRasterRendererAPI::DrawIndexed(int32_t indexCount, const Ref<Shader>& pShader)
	{
		
	}
}