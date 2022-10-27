#include "smpch.h"
#include "directx11_buffer.h"

namespace smile::graphic
{
    D3D11_USAGE bufferUsageToDirectXType( BufferUsage buffer_usage )
    {
        switch ( buffer_usage )
        {
            case BufferUsage::Default:
                return D3D11_USAGE_DEFAULT;
            case BufferUsage::Immutable:
                return D3D11_USAGE_IMMUTABLE;
            case BufferUsage::Dynamic:
                return D3D11_USAGE_DYNAMIC;
            case BufferUsage::Staging:
                return D3D11_USAGE_STAGING;
            default:
                return D3D11_USAGE_DEFAULT;
        }
    }
}