#include "smpch.h"
#include "SmileRasterBuffer.h"
#include "SmileEngine/Core/SmileGame.h"

namespace Smile
{
	//*------------------------------------------------------------------------------------------------*//
	//*----------------------------------------- Vertex buffer ----------------------------------------*//
	//*------------------------------------------------------------------------------------------------*//

	SmileRasterVertexBuffer::SmileRasterVertexBuffer(const VertexBufferData& vertexBufferData)
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterVertexBuffer > Rendering context is not a SmileRaster Rendering Context");

		m_VertexBuffer = m_pSmileRasterContext->GetDeviceContext()->CreateVertexBuffer(vertexBufferData.pVertices, vertexBufferData.Count, vertexBufferData.BufferLayout.GetStride());
		m_Layout = vertexBufferData.BufferLayout;
	}

	void SmileRasterVertexBuffer::Bind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetVertexBuffer(m_VertexBuffer);
	}

	void SmileRasterVertexBuffer::Unbind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetVertexBuffer(INVALID_SM_BUFFER_ID);
	}

	//*------------------------------------------------------------------------------------------------*//
	//*----------------------------------------- Index buffer -----------------------------------------*//
	//*------------------------------------------------------------------------------------------------*//

	SmileRasterIndexBuffer::SmileRasterIndexBuffer(const IndexBufferData& indexBufferData)
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterIndexBuffer > Rendering context is not a SmileRaster Rendering Context");
		
		m_IndexBuffer = m_pSmileRasterContext->GetDeviceContext()->CreateIndexBuffer(indexBufferData.pIndices, indexBufferData.Count);
		m_Count = indexBufferData.Count;
	}

	void SmileRasterIndexBuffer::Bind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetIndexBuffer(m_IndexBuffer);
	}

	void SmileRasterIndexBuffer::Unbind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetIndexBuffer(INVALID_SM_BUFFER_ID);
	}
}