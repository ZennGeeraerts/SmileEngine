#pragma once
#include "SmileEngine/Renderer/Buffer.h"

#include <d3d11.h>

namespace smile
{
    D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage );
}