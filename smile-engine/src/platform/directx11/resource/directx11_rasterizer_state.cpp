#include "smpch.h"
#include "directx11_rasterizer_state.h"

namespace smile::graphic
{
    DirectX11RasterizerState::~DirectX11RasterizerState()
    {
        SAFE_RELEASE( pInternal );
    }
}