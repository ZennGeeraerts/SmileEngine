#pragma once
#include "StaticMeshFilter.h"

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

	class SkinnedMeshFilter final : public StaticMeshFilter
	{
	public:
		SkinnedMeshFilter() = default;
		virtual ~SkinnedMeshFilter();
		
		virtual void Create(const BufferLayout& layout) override;
		bool HasAnimations() const { return m_bHasAnimations; }

	private:
		std::vector<DirectX::XMFLOAT4> m_BlendIndices = {};
		std::vector<DirectX::XMFLOAT4> m_BlendWeights = {};

		bool m_bHasAnimations = false;
		std::vector<AnimationClip> m_AnimationClips{};
		std::unordered_map<std::string, BoneInfo> m_SkeletonMap{};
		uint32_t m_BoneCount = 0;

		bool m_bUseBlendIndices = false;
		bool m_bUseBlendWeights = false;

		friend class MeshAnimator;
		friend class MeshLoader;
	};
}
