#include "smpch.h"
#include "DirectX11VertexBuffer.h"

#include "SmileEngine/Core/Application.h"
#include "DirectX11Diagnostics.h"
#include "DirectX11Buffer.h"

namespace smile
{
    DirectX11VertexBuffer::DirectX11VertexBuffer( const VertexBufferDescriptor &vertexBufferDesc )
        : m_Stride{ vertexBufferDesc.m_Stride }
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT(
            m_pDirectX11Context, "DirectX11VertexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = BufferUsageToDirectXType( vertexBufferDesc.m_Usage );
        bd.ByteWidth = vertexBufferDesc.m_Stride * vertexBufferDesc.m_Count;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = vertexBufferDesc.m_pVertices;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
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
        uint32_t offset{ 0 };
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_Stride, &offset );
    }

    void DirectX11VertexBuffer::Unbind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }
}