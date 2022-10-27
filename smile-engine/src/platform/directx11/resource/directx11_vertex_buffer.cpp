#include "smpch.h"
#include "directX11_vertex_buffer.h"

#include "smile_engine/core/application.h"
#include "platform/directX11/directx11_diagnostics.h"
#include "platform/directX11/shader/directx11_buffer.h"

namespace smile::graphic
{
    DirectX11VertexBuffer::DirectX11VertexBuffer( const VertexBufferDescriptor &vertex_buffer_desc )
        : stride{ vertex_buffer_desc.stride }
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT(
            directX11Context, "DirectX11VertexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage = bufferUsageToDirectXType( vertex_buffer_desc.usage );
        buffer_desc.ByteWidth = vertex_buffer_desc.stride * vertex_buffer_desc.count;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA init_data = { 0 };
        init_data.pSysMem = vertex_buffer_desc.vertices;

        HRESULT result = directX11Context->getDevice()->CreateBuffer( &buffer_desc, &init_data, &vertexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11VertexBuffer > Failed to create vertex buffer: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }
    }

    DirectX11VertexBuffer::~DirectX11VertexBuffer()
    {
        SAFE_RELEASE( vertexBuffer );
    }

    void DirectX11VertexBuffer::bind() const
    {
        Uint32 offset{ 0 };
        directX11Context->getDeviceContext()->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
    }

    void DirectX11VertexBuffer::unbind() const
    {
        directX11Context->getDeviceContext()->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }
}