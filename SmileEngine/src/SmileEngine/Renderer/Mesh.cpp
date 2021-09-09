#include "smpch.h"
#include "Mesh.h"

namespace Smile
{
	DirectX::XMFLOAT4 Mesh::m_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
	DirectX::XMFLOAT4 Mesh::m_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
	DirectX::XMFLOAT3 Mesh::m_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
	DirectX::XMFLOAT2 Mesh::m_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };

	Mesh::Mesh()
	{
		
	}

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

		VertexBufferData vertexBufferData{};
		vertexBufferData.pVertices = pDataLocation;
		vertexBufferData.Count = m_VertexCount;
		vertexBufferData.Usage = BufferUsage::eImmutable;
		vertexBufferData.BufferLayout = layout;

		for (int32_t i{}; i < m_VertexCount; ++i)
		{
			for (const BufferElement& element : layout)
			{
				if (element.Name == "Position")
					memcpy(pDataLocation, bUsePositions ? &m_Positions[i] : &m_DefaultFloat3, element.Size);
				else if (element.Name == "Normal")
					memcpy(pDataLocation, bUseNormals ? &m_Normals[i] : &m_DefaultFloat3, element.Size);

				pDataLocation = (char*)pDataLocation + element.Size;
			}
		}

		IndexBufferData indexBufferData{};
		indexBufferData.pIndices = m_Indices.data();
		indexBufferData.Count = m_Indices.size();
		indexBufferData.Usage = BufferUsage::eImmutable;

		std::string shaderFilePath = "../SmileProject/Resources/Shaders/Mesh.fx";

		m_pVertexBuffer.reset(VertexBuffer::Create(vertexBufferData));
		m_pIndexBuffer.reset(IndexBuffer::Create(indexBufferData));
		m_pShader.reset(Shader::Create(shaderFilePath, layout));
	}
}