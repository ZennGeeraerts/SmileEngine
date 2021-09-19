#include "smpch.h"
#include "Mesh.h"

namespace Smile
{
	DirectX::XMFLOAT4 Mesh::m_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
	DirectX::XMFLOAT4 Mesh::m_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
	DirectX::XMFLOAT3 Mesh::m_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
	DirectX::XMFLOAT2 Mesh::m_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };

	Mesh::~Mesh()
	{
		m_Positions.clear();
		m_Normals.clear();
		m_Tangents.clear();
		m_Binormals.clear();
		m_TexCoords.clear();
		m_Indices.clear();
	}

	void Mesh::Create(const BufferLayout& layout)
	{
		void* pDataLocation = malloc(layout.GetStride() * m_VertexCount);
		if (!pDataLocation)
		{
			SM_LOG_ERROR("Mesh::Create > Failed to allocate memory for the vertex buffer");
			return;
		}

		VertexBufferData vertexBufferData{};
		vertexBufferData.pVertices = pDataLocation;
		vertexBufferData.Count = m_VertexCount;
		vertexBufferData.Usage = BufferUsage::eImmutable;
		vertexBufferData.BufferLayout = layout;

		for (uint32_t i{}; i < m_VertexCount; ++i)
		{
			for (const BufferElement& element : layout)
			{
				if (element.Name == "Position")
					memcpy(pDataLocation, bUsePositions ? &m_Positions[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Normal")
					memcpy(pDataLocation, bUseNormals ? &m_Normals[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "TexCoord")
					memcpy(pDataLocation, bUseTexCoords ? &m_TexCoords[i] : &m_DefaultFloat2, element.Size);
				else if (element.Name == "Tangent")
					memcpy(pDataLocation, bUseTangents ? &m_Tangents[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Binormal")
					memcpy(pDataLocation, bUseBinormals ? &m_Binormals[i] : &m_DefaultFloat3, element.Size);

				pDataLocation = (char*)pDataLocation + element.Size;
			}
		}

		IndexBufferData indexBufferData{};
		indexBufferData.pIndices = m_Indices.data();
		indexBufferData.Count = static_cast<uint32_t>(m_Indices.size());
		indexBufferData.Usage = BufferUsage::eImmutable;

		std::string shaderFilePath = "../SmileProject/Resources/Shaders/PBR.fx";

		m_pVertexBuffer.reset(VertexBuffer::Create(vertexBufferData));
		m_pIndexBuffer.reset(IndexBuffer::Create(indexBufferData));
		m_pShader = Shader::Create(shaderFilePath, layout);
	}
}