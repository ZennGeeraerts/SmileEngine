#include "smpch.h"
#include "directx11_index_buffer.h"
#include "smile_engine/core/application.h"

#include "platform/directx11/directx11_diagnostics.h"
#include "platform/directX11/shader/directx11_buffer.h"

namespace smile::renderer
{
    DirectX11IndexBuffer::DirectX11IndexBuffer( const IndexBufferDescriptor &index_buffer_desc )
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT(
            directX11Context, "DirectX11IndexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        count = index_buffer_desc.count;

        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage = bufferUsageToDirectXType( index_buffer_desc.usage );
        buffer_desc.ByteWidth = sizeof( Uint32 ) * index_buffer_desc.count;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA init_data = { 0 };
        init_data.pSysMem = index_buffer_desc.indices;

        HRESULT result = directX11Context->getDevice()->CreateBuffer( &buffer_desc, &init_data, &indexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11IndexBuffer > Failed to create index buffer: %ls", getDirectX11ErrorMessage( result ) );
            return;
        }
    }

    DirectX11IndexBuffer::~DirectX11IndexBuffer()
    {
        SAFE_RELEASE( indexBuffer );
    }

    void DirectX11IndexBuffer::bind() const
    {
        directX11Context->getDeviceContext()->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11IndexBuffer::unbind() const
    {
        directX11Context->getDeviceContext()->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }
}