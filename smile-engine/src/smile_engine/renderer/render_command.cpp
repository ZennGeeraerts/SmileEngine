#include "smpch.h"
#include "render_command.h"

#include "platform/directx11/directx11_renderer_api.h"
#include "platform/smileraster/smileraster_renderer_api.h"

namespace smile
{
    RendererAPI *RenderCommand::s_pRendererAPI = new DirectX11RendererAPI{};
}