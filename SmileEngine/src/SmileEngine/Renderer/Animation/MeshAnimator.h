#pragma once
#include "SmileEngine/Renderer/MeshFilter.h"

#define MAX_BONE_COUNT 70

namespace Smile
{
	class MeshAnimator final
	{
	public:
		MeshAnimator(const Ref<MeshFilter>& pMesh);
		virtual ~MeshAnimator() = default;

		void OnUpdate(Timestep deltaTime);

		void SetAnimation(const std::string& clipName);
		void SetAnimation(uint32_t clipID);
		void SetAnimation(const AnimationClip& clip);

		void Play() { m_bPlaying = true; }
		void Pause() { m_bPlaying = false; }
		void Reset(bool bPause = true);

		const std::vector<DirectX::XMFLOAT4X4>& GetBoneTransforms() const { return m_Transforms; }
		bool IsPlaying() const { return m_bPlaying; }

	private:
		void CalculateBoneTransform(AnimationNode* pNode, const DirectX::XMFLOAT4X4& parentTransform);

	private:
		AnimationClip m_CurrentClip;
		Ref<MeshFilter> m_pMesh;
		std::vector<DirectX::XMFLOAT4X4> m_Transforms;

		float m_TickCount = 0.f;

		bool m_bClipSet = false;
		bool m_bPlaying = false;
		bool m_bReversed = false;
	};
}

