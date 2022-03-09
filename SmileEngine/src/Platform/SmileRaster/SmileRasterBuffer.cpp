#include "smpch.h"
#include "SmileRasterBuffer.h"
#include "SmileEngine/Core/Application.h"

namespace smile
{
    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Vertex buffer ----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterVertexBuffer::SmileRasterVertexBuffer( const VertexBufferDescriptor &vertexBufferDescriptor )
        : m_Stride{ vertexBufferDescriptor.m_Stride }
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext,
            "SmileRasterVertexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        m_VertexBuffer =
            m_pSmileRasterContext->GetDeviceContext()->CreateVertexBuffer( vertexBufferDescriptor.m_pVertices,
                vertexBufferDescriptor.m_Count,
                vertexBufferDescriptor.m_Stride * vertexBufferDescriptor.m_Count );
    }

    void SmileRasterVertexBuffer::Bind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindVertexBuffer( m_VertexBuffer, m_Stride );
    }

    void SmileRasterVertexBuffer::Unbind() const
    {
        m_pSmileRasterContext->GetDeviceContext()->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
    }

    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Index buffer -----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterIndexBuffer::SmileRasterIndexBuffer( const IndexBufferDescriptor &indexBufferDescriptor )
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext,
            "SmileRasterIndexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        m_IndexBuffer = m_pSmileRasterContext->GetDeviceContext()->CreateIndexBuffer(
            indexBufferDescriptor.m_pIndices, indexBufferDescriptor.m_Count );
        m_Count = indexBufferDescriptor.m_Count;
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