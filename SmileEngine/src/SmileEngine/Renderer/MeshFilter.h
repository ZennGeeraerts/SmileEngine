#pragma once
#include "Buffer.h"
#include "Shader.h"
#include "Animation/Bone.h"

namespace Smile
{
	struct BoneInfo final
	{
		uint32_t ID{};
		DirectX::XMFLOAT4X4 Offset{};
	};

	struct AnimationNode final
	{
		DirectX::XMFLOAT4X4 Transform{};
		std::string Name{};
		uint32_t ChildrenCount{};
		std::vector<AnimationNode> Children;
	};

	struct AnimationClip final
	{
		std::string Name{};
		float Duration{};
		float TicksPerSecond{};
		std::vector<Bone> Bones{};
		AnimationNode RootNode{};
	};

	class MeshFilter final
	{
	public:
		MeshFilter() = default;
		~MeshFilter();

		void Create(const BufferLayout& layout);

		Ref<VertexBuffer> GetVertexBuffer() const { return m_pVertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_pIndexBuffer; }
		const std::string& GetFilePath() const { return m_FilePath; }
		bool HasAnimations() const { return m_bHasAnimations; }

	private:
		std::vector<DirectX::XMFLOAT3> m_Positions = {};
		std::vector<DirectX::XMFLOAT3> m_Normals = {};
		std::vector<DirectX::XMFLOAT3> m_Tangents = {};
		std::vector<DirectX::XMFLOAT3> m_Binormals = {};
		std::vector<DirectX::XMFLOAT2> m_TexCoords = {};
		std::vector<DirectX::XMFLOAT4> m_Colors = {};
		std::vector<DirectX::XMFLOAT4> m_BlendIndices = {};
		std::vector<DirectX::XMFLOAT4> m_BlendWeights = {};

		bool m_bHasAnimations = false;
		std::vector<AnimationClip> m_AnimationClips{};
		std::unordered_map<std::string, BoneInfo> m_SkeletonMap{};
		uint32_t m_BoneCount = 0;

		bool m_bUsePositions = false;
		bool m_bUseNormals = false;
		bool m_bUseTangents = false;
		bool m_bUseBinormals = false;
		bool m_bUseTexCoords = false;
		bool m_bUseColors = false;
		bool m_bUseBlendIndices = false;
		bool m_bUseBlendWeights = false;

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

		friend class MeshLoader;
		friend class MeshAnimator;
	};
}

