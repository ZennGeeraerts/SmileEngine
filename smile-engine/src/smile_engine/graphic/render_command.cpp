#include "smpch.h"
#include "render_command.h"

#include "platform/directx11/directx11_renderer_api.h"
#include "platform/smileraster/smileraster_renderer_api.h"

namespace Smile::Graphic
{
    RendererAPI *RenderCommand::s_RendererAPI = new DirectX11RendererAPI{};
}