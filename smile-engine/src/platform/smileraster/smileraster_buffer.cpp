#include "smpch.h"
#include "smileraster_buffer.h"
#include "smile_engine/core/application.h"

namespace smile::renderer
{
    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Vertex buffer ----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterVertexBuffer::SmileRasterVertexBuffer( const VertexBufferDescriptor &vertex_buffer_descriptor )
        : stride{ vertex_buffer_descriptor.stride }
    {
        smileRasterContext =
            static_cast< SmileRasterContext * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( smileRasterContext,
            "SmileRasterVertexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        vertexBuffer =
            smileRasterContext->getDeviceContext()->CreateVertexBuffer( vertex_buffer_descriptor.vertices,
                vertex_buffer_descriptor.count,
                vertex_buffer_descriptor.stride * vertex_buffer_descriptor.count );
    }

    void SmileRasterVertexBuffer::bind() const
    {
        smileRasterContext->getDeviceContext()->BindVertexBuffer( vertexBuffer, stride );
    }

    void SmileRasterVertexBuffer::unbind() const
    {
        smileRasterContext->getDeviceContext()->BindVertexBuffer( SMR_INVALID_BUFFER_ID, 0 );
    }

    //*------------------------------------------------------------------------------------------------*//
    //*----------------------------------------- Index buffer -----------------------------------------*//
    //*------------------------------------------------------------------------------------------------*//

    SmileRasterIndexBuffer::SmileRasterIndexBuffer( const IndexBufferDescriptor &index_buffer_descriptor )
    {
        smileRasterContext =
            static_cast< SmileRasterContext * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( smileRasterContext,
            "SmileRasterIndexBuffer > Rendering context is not a SmileRaster Rendering Context" );

        indexBuffer = smileRasterContext->getDeviceContext()->CreateIndexBuffer(
            index_buffer_descriptor.indices, index_buffer_descriptor.count );
        count = index_buffer_descriptor.count;
    }

    void SmileRasterIndexBuffer::bind() const
    {
        smileRasterContext->getDeviceContext()->BindIndexBuffer( indexBuffer );
    }

    void SmileRasterIndexBuffer::unbind() const
    {
        smileRasterContext->getDeviceContext()->BindIndexBuffer( SMR_INVALID_BUFFER_ID );
    }
}