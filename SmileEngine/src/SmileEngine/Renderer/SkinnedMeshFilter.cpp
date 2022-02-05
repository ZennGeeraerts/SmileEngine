#include "smpch.h"
#include "SkinnedMeshFilter.h"

namespace Smile
{
	SkinnedMeshFilter::~SkinnedMeshFilter()
	{
		m_BlendIndices.clear();
		m_BlendWeights.clear();
		m_SkeletonMap.clear();
	}

	void SkinnedMeshFilter::Create(const BufferLayout& layout)
	{
		m_pDataLocation = malloc(layout.GetStride() * m_VertexCount);
		if (!m_pDataLocation)
		{
			SM_LOG_ERROR("SkinnedMeshFilter::Create > Failed to allocate memory for the vertex buffer");
			return;
		}

		VertexBufferData vertexBufferData{};
		vertexBufferData.pVertices = m_pDataLocation;
		vertexBufferData.Count = m_VertexCount;
		vertexBufferData.Usage = BufferUsage::eImmutable;
		vertexBufferData.BufferLayout = layout;

		for (uint32_t i{}; i < m_VertexCount; ++i)
		{
			for (const BufferElement& element : layout)
			{
				if (element.Name == "Position")
					memcpy(m_pDataLocation, m_bUsePositions ? &m_Positions[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Normal")
					memcpy(m_pDataLocation, m_bUseNormals ? &m_Normals[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "TexCoord")
					memcpy(m_pDataLocation, m_bUseTexCoords ? &m_TexCoords[i] : &m_DefaultFloat2, element.Size);
				else if (element.Name == "Tangent")
					memcpy(m_pDataLocation, m_bUseTangents ? &m_Tangents[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Binormal")
					memcpy(m_pDataLocation, m_bUseBinormals ? &m_Binormals[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Color")
					memcpy(m_pDataLocation, m_bUseColors ? &m_Colors[i] : &m_DefaultFloat4, element.Size);
				else if (element.Name == "BlendIndices")
					memcpy(m_pDataLocation, m_bUseBlendIndices ? &m_BlendIndices[i] : &m_DefaultIndices4, element.Size);
				else if (element.Name == "BlendWeights")
					memcpy(m_pDataLocation, m_bUseBlendWeights ? &m_BlendWeights[i] : &m_DefaultFloat4, element.Size);

				m_pDataLocation = (char*)m_pDataLocation + element.Size;
			}
		}

		IndexBufferData indexBufferData{};
		indexBufferData.pIndices = m_Indices.data();
		indexBufferData.Count = static_cast<uint32_t>(m_Indices.size());
		indexBufferData.Usage = BufferUsage::eImmutable;

		m_pVertexBuffer.reset(VertexBuffer::Create(vertexBufferData));
		m_pIndexBuffer.reset(IndexBuffer::Create(indexBufferData));
	}
}
