#include "smpch.h"
#include "SmileRasterBuffer.h"
#include "SmileEngine/Core/Application.h"

namespace smile
{
    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Vertex buffer ----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterVertexBuffer::SmileRasterVertexBuffer( const VertexBufferData &vertexBufferData )
        : m_Layout{ vertexBufferData.m_BufferLayout }
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext,
            "SmileRasterVertexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        m_VertexBuffer = m_pSmileRasterContext->GetDeviceContext()->CreateVertexBuffer( vertexBufferData.m_pVertices,
            vertexBufferData.m_Count,
            vertexBufferData.m_BufferLayout.GetStride() * vertexBufferData.m_Count );
    }

    void SmileRasterVertexBuffer::Bind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindVertexBuffer( m_VertexBuffer, m_Layout.GetStride() );
    }

    void SmileRasterVertexBuffer::Unbind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
    }

    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Index buffer -----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterIndexBuffer::SmileRasterIndexBuffer( const IndexBufferData &indexBufferData )
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext,
            "SmileRasterIndexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        m_IndexBuffer = m_pSmileRasterContext->GetDeviceContext()->CreateIndexBuffer(
            indexBufferData.m_pIndices, indexBufferData.m_Count );
        m_Count = indexBufferData.m_Count;
    }

    void SmileRasterIndexBuffer::Bind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindIndexBuffer( m_IndexBuffer );
    }

    void SmileRasterIndexBuffer::Unbind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindIndexBuffer( SMR_INVALID_BUFFER_ID );
    }
}