#include "smpch.h"
#include "MeshAnimator.h"

namespace Smile
{
	MeshAnimator::MeshAnimator(const Ref<MeshFilter>& pMesh)
		: m_pMesh{ pMesh }
	{
		m_Transforms.resize(70);
	}

	void MeshAnimator::OnUpdate(Timestep deltaTime)
	{
		if (m_bClipSet && m_bPlaying)
		{
			float passedTicks = deltaTime * m_CurrentClip.TicksPerSecond;
			passedTicks = fmod(passedTicks, m_CurrentClip.Duration);
			
			if (!m_bReversed)
			{
				m_TickCount += passedTicks;
				if (m_TickCount > m_CurrentClip.Duration)
					m_TickCount = 0;
			}
			else
			{
				m_TickCount -= passedTicks;
				if (m_TickCount < 0)
					m_TickCount = m_CurrentClip.Duration;
			}

			DirectX::XMFLOAT4X4 identityMatrix{};
			DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
			CalculateBoneTransform(&m_CurrentClip.RootNode, identityMatrix);
		}
	}

	void MeshAnimator::CalculateBoneTransform(AnimationNode* pNode, const DirectX::XMFLOAT4X4& parentTransform)
	{
		const std::string& nodeName = pNode->Name;
		DirectX::XMFLOAT4X4 nodeTransform = pNode->Transform;

		auto it = std::find_if(m_CurrentClip.Bones.begin(), m_CurrentClip.Bones.end(),
			[nodeName] (const Bone& bone)
			{
				return bone.GetName() == nodeName;
			});
	
		if (it != m_CurrentClip.Bones.end())
		{
			Bone& bone = (*it);
			bone.OnUpdate(m_TickCount);
			nodeTransform = bone.GetLocalTransform();
		}

		DirectX::XMMATRIX globalTransformMat = DirectX::XMLoadFloat4x4(&nodeTransform) * DirectX::XMLoadFloat4x4(&parentTransform);
		DirectX::XMFLOAT4X4 globalTransform{};
		DirectX::XMStoreFloat4x4(&globalTransform, globalTransformMat);

		if (m_pMesh->m_BoneMap.find(nodeName) != m_pMesh->m_BoneMap.end())
		{
			uint32_t id = m_pMesh->m_BoneMap[nodeName].ID;
			DirectX::XMMATRIX transformMat = DirectX::XMLoadFloat4x4(&m_pMesh->m_BoneMap[nodeName].Offset) * globalTransformMat;
			DirectX::XMStoreFloat4x4(&m_Transforms[id], transformMat);
		}

		for (uint32_t i{}; i < pNode->ChildrenCount; ++i)
			CalculateBoneTransform(&pNode->Children[i], globalTransform);
	}

	void MeshAnimator::SetAnimation(const std::string& clipName)
	{
		m_bClipSet = false;

		auto it = std::find_if(m_pMesh->m_AnimationClips.begin(), m_pMesh->m_AnimationClips.end(),
			[clipName] (const AnimationClip& clip)
			{
				return clip.Name == clipName;
			});

		if (it != m_pMesh->m_AnimationClips.end())
			SetAnimation(*it);
		else
		{
			Reset(false);
			SM_LOG_WARNING("MeshAnimator::SetAnimation > clip with clipName was not found in m_AnimationClips");
		}
	}

	void MeshAnimator::SetAnimation(uint32_t clipID)
	{
		if (clipID < m_pMesh->m_AnimationClips.size())
		{
			AnimationClip& animationClip = m_pMesh->m_AnimationClips[clipID];
			SetAnimation(animationClip);
		}
		else
		{
			Reset(false);
			SM_LOG_WARNING("MeshAnimator::SetAnimation > clipID is bigger than the number of animation clips");
		}
	}

	void MeshAnimator::SetAnimation(const AnimationClip& clip)
	{
		m_bClipSet = true;
		m_CurrentClip = clip;
		Reset(false);
	}

	void MeshAnimator::Reset(bool bPause)
	{
		//if (bPause)
		//	Pause();

		////Set m_TickCount to zero
		//m_TickCount = 0;
		////Set m_AnimationSpeed to 1.0f
		//m_AnimationSpeed = 1.0f;

		////If m_ClipSet is true
		////	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		////	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		////Else
		////	Create an IdentityMatrix 
		////	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		//if (m_bClipSet)
		//{
		//	auto boneTransforms = m_CurrentClip.Keys[0].BoneTransforms;
		//	m_Transforms.assign(boneTransforms.begin(), boneTransforms.end());
		//}
		//else
		//{
		//	DirectX::XMFLOAT4X4 identityMatrix{};
		//	DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity{});

		//	m_Transforms.assign(m_pMesh->m_BoneCount, identityMatrix);
		//}
	}
}