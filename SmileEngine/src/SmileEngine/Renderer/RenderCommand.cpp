#include "smpch.h"
#include "RenderCommand.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"
#include "Platform/SmileRaster/SmileRasterRendererAPI.h"

namespace smile
{
    RendererAPI *RenderCommand::s_pRendererAPI = new DirectX11RendererAPI{};
}