#include "smpch.h"
#include "directx11_index_buffer.h"
#include "smile_engine/core/application.h"

#include "platform/directx11/directx11_diagnostics.h"
#include "platform/directX11/shader/directx11_buffer.h"

namespace smile
{
    DirectX11IndexBuffer::DirectX11IndexBuffer( const IndexBufferDescriptor &indexBufferDesc )
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT(
            m_pDirectX11Context, "DirectX11IndexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        m_Count = indexBufferDesc.m_Count;

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = BufferUsageToDirectXType( indexBufferDesc.m_Usage );
        bd.ByteWidth = sizeof( Uint32 ) * indexBufferDesc.m_Count;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = indexBufferDesc.m_pIndices;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11IndexBuffer > Failed to create index buffer: %ls", GetDirectX11ErrorMessage( result ) );
            return;
        }
    }

    DirectX11IndexBuffer::~DirectX11IndexBuffer()
    {
        SAFE_RELEASE( m_pIndexBuffer );
    }

    void DirectX11IndexBuffer::Bind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    }

    void DirectX11IndexBuffer::Unbind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetIndexBuffer( nullptr, DXGI_FORMAT_UNKNOWN, 0 );
    }
}