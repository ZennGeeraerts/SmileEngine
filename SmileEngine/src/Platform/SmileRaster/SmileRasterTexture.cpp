#include "smpch.h"
#include "SmileRasterTexture.h"

#include "SmileEngine/Core/Logger.h"
#include "SmileEngine/Core/Application.h"

namespace Smile
{
	SmileRasterTexture2D::SmileRasterTexture2D(const std::string& filePath)
		: m_FilePath{ filePath }
		, m_pSurface{ IMG_Load(filePath.c_str()) }
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(Application::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterTexture2D > Rendering context is not a SmileRasterContext");

		if (!m_pSurface)
			return;

		m_pPixels = static_cast<uint8_t*>(m_pSurface->pixels);
		m_Width = m_pSurface->w;
		m_Height = m_pSurface->h;

		m_Texture = m_pSmileRasterContext->GetDeviceContext()->CreateTexture2D(m_pPixels, m_Width, m_Height);
	}

	SmileRasterTexture2D::~SmileRasterTexture2D()
	{
		SDL_FreeSurface(m_pSurface);
	}
}