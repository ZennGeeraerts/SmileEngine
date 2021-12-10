#include "smpch.h"
#include "RenderCommand.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"
#include "Platform/SmileRaster/SmileRasterRendererAPI.h"

namespace Smile
{
	RendererAPI* RenderCommand::m_pRendererAPI = new SmileRasterRendererAPI{};
}