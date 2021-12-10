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

		m_Layout = vertexBufferData.BufferLayout;
		m_pVertices = vertexBufferData.pVertices;
		m_Count = vertexBufferData.Count;
	}

	void SmileRasterVertexBuffer::Bind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetVertexBuffer(m_pVertices, m_Count, m_Layout.GetStride());
	}

	void SmileRasterVertexBuffer::Unbind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetVertexBuffer(nullptr, 0, 0);
	}

	//*------------------------------------------------------------------------------------------------*//
	//*----------------------------------------- Index buffer -----------------------------------------*//
	//*------------------------------------------------------------------------------------------------*//

	SmileRasterIndexBuffer::SmileRasterIndexBuffer(const IndexBufferData& indexBufferData)
	{
		m_pSmileRasterContext = static_cast<SmileRasterContext*>(SmileGame::GetInstance().GetWindow().GetRenderingContext());
		SM_ASSERT(m_pSmileRasterContext, "SmileRasterIndexBuffer > Rendering context is not a SmileRaster Rendering Context");
		
		m_pIndices = indexBufferData.pIndices;
		m_Count = indexBufferData.Count;
	}

	void SmileRasterIndexBuffer::Bind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetIndexBuffer(m_pIndices, m_Count);
	}

	void SmileRasterIndexBuffer::Unbind() const
	{
		m_pSmileRasterContext->GetDeviceContext()->SetIndexBuffer(nullptr, 0);
	}
}