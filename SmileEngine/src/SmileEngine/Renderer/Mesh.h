#pragma once
#include "Buffer.h"
#include "Shader.h"

namespace Smile
{
	class Mesh final
	{
	public:
		Mesh() = default;
		~Mesh();

		void Create(const BufferLayout& layout);

		Ref<VertexBuffer> GetVertexBuffer() const { return m_pVertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_pIndexBuffer; }
		const std::string& GetFilePath() const { return m_FilePath; }

	private:
		std::vector<DirectX::XMFLOAT3> m_Positions = {};
		std::vector<DirectX::XMFLOAT3> m_Normals = {};
		std::vector<DirectX::XMFLOAT3> m_Tangents = {};
		std::vector<DirectX::XMFLOAT3> m_Binormals = {};
		std::vector<DirectX::XMFLOAT2> m_TexCoords = {};

		bool m_bUsePositions = false;
		bool m_bUseNormals = false;
		bool m_bUseTangents = false;
		bool m_bUseBinormals = false;
		bool m_bUseTexCoords = false;

		std::vector<uint32_t> m_Indices = {};

		Ref<VertexBuffer> m_pVertexBuffer = nullptr;
		Ref<IndexBuffer> m_pIndexBuffer = nullptr;

		uint32_t m_VertexCount = 0;

		std::string m_FilePath = {};

		static DirectX::XMFLOAT4 m_DefaultColor;
		static DirectX::XMFLOAT4 m_DefaultFloat4;
		static DirectX::XMFLOAT3 m_DefaultFloat3;
		static DirectX::XMFLOAT2 m_DefaultFloat2;

		friend class MeshLoader;
	};
}

