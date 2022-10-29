#include "smpch.h"
#include "smileraster_buffer.h"
#include "smile_engine/core/application.h"

namespace Smile::Graphic
{
    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Vertex buffer ----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterVertexBuffer::SmileRasterVertexBuffer( const VertexBufferDescriptor &vertexBufferDescriptor )
        : m_Stride{ vertexBufferDescriptor.Stride }
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pSmileRasterContext,
            "SmileRasterVertexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        m_VertexBuffer =
            m_pSmileRasterContext->getDeviceContext()->CreateVertexBuffer( vertexBufferDescriptor.pVertices,
                vertexBufferDescriptor.Count,
                vertexBufferDescriptor.Stride * vertexBufferDescriptor.Count );
    }

    void SmileRasterVertexBuffer::Bind() const
    {
        m_pSmileRasterContext->getDeviceContext()->BindVertexBuffer( m_VertexBuffer, m_Stride );
    }

    void SmileRasterVertexBuffer::Unbind() const
    {
        m_pSmileRasterContext->getDeviceContext()->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
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

        m_IndexBuffer = m_pSmileRasterContext->getDeviceContext()->CreateIndexBuffer(
            indexBufferDescriptor.pIndices, indexBufferDescriptor.Count );
        m_Count = indexBufferDescriptor.Count;
    }

    void SmileRasterIndexBuffer::Bind() const
    {
        m_pSmileRasterContext->getDeviceContext()->BindIndexBuffer( m_IndexBuffer );
    }

    void SmileRasterIndexBuffer::Unbind() const
    {
        m_pSmileRasterContext->getDeviceContext()->BindIndexBuffer( SMR_INVALID_BUFFER_ID );
    }
}