#include "smpch.h"
#include "DirectX11Buffer.h"
#include "SmileEngine/Core/Application.h"

namespace smile
{
    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Vertex buffer ----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    DirectX11VertexBuffer::DirectX11VertexBuffer( const VertexBufferData &vertexBufferData )
        : m_Layout{ vertexBufferData.m_BufferLayout }
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetRenderingContext() );
        SM_ASSERT(
            m_pDirectX11Context, "DirectX11VertexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = BufferUsageToDirectXType( vertexBufferData.m_Usage );
        bd.ByteWidth = vertexBufferData.m_BufferLayout.GetStride() * vertexBufferData.m_Count;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = vertexBufferData.m_pVertices;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11VertexBuffer > Failed to create vertex buffer" );
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
        uint32_t stride{ m_Layout.GetStride() };
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
    }

    void DirectX11VertexBuffer::Unbind() const
    {
        m_pDirectX11Context->GetDeviceContext()->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
    }

    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Index buffer -----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    DirectX11IndexBuffer::DirectX11IndexBuffer( const IndexBufferData &indexBufferData )
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetRenderingContext() );
        SM_ASSERT(
            m_pDirectX11Context, "DirectX11IndexBuffer > Rendering context is not a DirectX 11 Rendering Context" );

        m_Count = indexBufferData.m_Count;

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = BufferUsageToDirectXType( indexBufferData.m_Usage );
        bd.ByteWidth = sizeof( uint32_t ) * indexBufferData.m_Count;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = { 0 };
        initData.pSysMem = indexBufferData.m_pIndices;

        HRESULT result = m_pDirectX11Context->GetDevice()->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11IndexBuffer > Failed to create index buffer" );
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

    //*------------------------------------------------------------------------------------------------*//
    //*--------------------------------------- Helper functions ---------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage )
    {
        switch ( bufferUsage )
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