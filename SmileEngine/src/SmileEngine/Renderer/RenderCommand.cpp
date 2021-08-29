#include "smpch.h"
#include "RenderCommand.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"

namespace Smile
{
	RendererAPI* RenderCommand::m_pRendererAPI = new DirectX11RendererAPI{};
}