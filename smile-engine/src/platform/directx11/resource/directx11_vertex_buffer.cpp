#include "smpch.h"
#include "directX11_vertex_buffer.h"

#include "smile_engine/core/application.h"
#include "platform/directX11/directx11_diagnostics.h"
#include "platform/directX11/shader/directx11_buffer.h"

namespace Smile::Graphic
{
    DirectX11VertexBuffer::DirectX11VertexBuffer( const VertexBufferDescriptor &vertexBufferDesc )
        : m_Stride{ vertexBufferDesc.Stride }
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT(
            m_pDirectX11Context, "DirectX11VertexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( vertexBufferDesc.Usage );
        bufferDesc.ByteWidth = vertexBufferDesc.Stride * vertexBufferDesc.Count;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = vertexBufferDesc.pVertices;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateBuffer( &bufferDesc, &initData, &m_pVertexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11VertexBuffer > Failed to create vertex buffer: %ls", GetDirectX11ErrorMessage( result ) );
            return;
        }
    }

    DirectX11VertexBuffer::~DirectX11VertexBuffer()
    {
        SAFE_RELEASE( m_pVertexBuffer );
    }

    void DirectX11VertexBuffer::Bind() const
    {
        Uint32 offset{ 0 };
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_Stride, &offset );
    }

    void DirectX11VertexBuffer::Unbind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }
}