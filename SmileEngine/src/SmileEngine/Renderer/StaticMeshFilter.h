#pragma once
#include "Buffer.h"
#include "Shader.h"
#include "Animation/Bone.h"

namespace Smile
{
	class StaticMeshFilter
	{
	public:
		StaticMeshFilter() = default;
		virtual ~StaticMeshFilter();

		virtual void Create(const BufferLayout& layout);

		Ref<VertexBuffer> GetVertexBuffer() const { return m_pVertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_pIndexBuffer; }
		const std::string& GetFilePath() const { return m_FilePath; }

	protected:
		std::vector<DirectX::XMFLOAT3> m_Positions = {};
		std::vector<DirectX::XMFLOAT3> m_Normals = {};
		std::vector<DirectX::XMFLOAT3> m_Tangents = {};
		std::vector<DirectX::XMFLOAT3> m_Binormals = {};
		std::vector<DirectX::XMFLOAT2> m_TexCoords = {};
		std::vector<DirectX::XMFLOAT4> m_Colors = {};

		bool m_bUsePositions = false;
		bool m_bUseNormals = false;
		bool m_bUseTangents = false;
		bool m_bUseBinormals = false;
		bool m_bUseTexCoords = false;
		bool m_bUseColors = false;

		std::vector<uint32_t> m_Indices = {};

		Ref<VertexBuffer> m_pVertexBuffer = nullptr;
		Ref<IndexBuffer> m_pIndexBuffer = nullptr;

		uint32_t m_VertexCount = 0;

		std::string m_FilePath = {};
		void* m_pDataLocation = nullptr;

		static DirectX::XMFLOAT4 m_DefaultColor;
		static DirectX::XMFLOAT4 m_DefaultFloat4;
		static DirectX::XMFLOAT3 m_DefaultFloat3;
		static DirectX::XMFLOAT2 m_DefaultFloat2;
		static DirectX::XMFLOAT4 m_DefaultIndices4;

	private:
		friend class MeshLoader;
	};
}

