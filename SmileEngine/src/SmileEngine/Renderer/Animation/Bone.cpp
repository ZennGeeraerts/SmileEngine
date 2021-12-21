#include "smpch.h"
#include "Bone.h"
#include "SmileEngine/Core/Logger.h"

namespace Smile
{
	Bone::Bone(const std::string& name, uint32_t id)
		: m_Name{ name }
		, m_ID{ id }
	{
		DirectX::XMStoreFloat4x4(&m_LocalTransform, DirectX::XMMatrixIdentity());
	}

	void Bone::OnUpdate(float animationTime)
	{
		DirectX::XMFLOAT3 translation{};
		InterpolateTranslation(translation, animationTime);
		DirectX::XMFLOAT4 rotation{};
		InterpolateRotation(rotation, animationTime);
		DirectX::XMFLOAT3 scale{};
		InterpolateScale(scale, animationTime);

		DirectX::XMMATRIX localTransformMat = DirectX::XMMatrixTransformation(DirectX::XMVectorZero(), DirectX::XMQuaternionIdentity(), DirectX::XMLoadFloat3(&scale), 
			DirectX::XMVectorZero(), DirectX::XMLoadFloat4(&rotation), 
			DirectX::XMLoadFloat3(&translation));

		DirectX::XMStoreFloat4x4(&m_LocalTransform, localTransformMat);
	}

	void Bone::InterpolateTranslation(DirectX::XMFLOAT3& translation, float animationTime)
	{
		if (m_TranslationCount == 0)
		{
			translation = DirectX::XMFLOAT3{ 0, 0, 0 };
			return;
		}

		if (m_TranslationCount == 1)
		{
			translation = { m_Translations[0].Translation.x,  m_Translations[0].Translation.y, m_Translations[0].Translation.z };
			return;
		}

		uint32_t p0Index = GetTranslationIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Translations[p0Index].Tick, m_Translations[p1Index].Tick, animationTime);

		DirectX::XMVECTOR finalTranslationVec = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&m_Translations[p0Index].Translation), DirectX::XMLoadFloat3(&m_Translations[p1Index].Translation), scaleFactor);
		DirectX::XMStoreFloat3(&translation, finalTranslationVec);
	}

	uint32_t Bone::GetTranslationIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_TranslationCount - 1); ++i)
		{
			if (animationTime <= m_Translations[static_cast<size_t>(i) + 1].Tick)
				return i;
		}
		
		SM_ASSERT(false, "Bone::GetTranslationIndex > Translation index is 0");
	}

	void Bone::InterpolateRotation(DirectX::XMFLOAT4& rotation, float animationTime)
	{
		if (m_RotationCount == 0)
		{
			DirectX::XMStoreFloat4(&rotation, DirectX::XMQuaternionIdentity());
			return;
		}

		if (m_RotationCount == 1)
		{
			DirectX::XMVECTOR rotationVec = DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&m_Rotations[0].Rotation));
			DirectX::XMStoreFloat4(&rotation, rotationVec);
			return;
		}

		uint32_t p0Index = GetRotationIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Rotations[p0Index].Tick, m_Rotations[p1Index].Tick, animationTime);

		DirectX::XMVECTOR finalRotationVec = DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&m_Rotations[p0Index].Rotation), DirectX::XMLoadFloat4(&m_Rotations[p1Index].Rotation), scaleFactor);
		finalRotationVec = DirectX::XMQuaternionNormalize(finalRotationVec);
		DirectX::XMStoreFloat4(&rotation, finalRotationVec);
	}

	uint32_t Bone::GetRotationIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_RotationCount - 1); ++i)
		{
			if (animationTime <= m_Rotations[static_cast<size_t>(i) + 1].Tick)
				return i;
		}

		SM_ASSERT(false, "Bone::GetRotationIndex > Rotation index is 0");
	}

	void Bone::InterpolateScale(DirectX::XMFLOAT3& scale, float animationTime)
	{
		if (m_ScaleCount == 0)
		{
			scale = DirectX::XMFLOAT3{ 1, 1, 1 };
			return;
		}

		if (m_ScaleCount == 1)
		{
			scale = { m_Scales[0].Scale.x, m_Scales[0].Scale.y, m_Scales[0].Scale.z };
			return;
		}

		uint32_t p0Index = GetScaleIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Scales[p0Index].Tick, m_Scales[p1Index].Tick, animationTime);
		DirectX::XMVECTOR finalScaleVec = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&m_Scales[p0Index].Scale), DirectX::XMLoadFloat3(&m_Scales[p1Index].Scale), scaleFactor);
		DirectX::XMStoreFloat3(&scale, finalScaleVec);
	}

	uint32_t Bone::GetScaleIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_ScaleCount - 1); ++i)
		{
			if (animationTime <= m_Scales[static_cast<size_t>(i) + 1].Tick)
				return i;
		}

		SM_ASSERT(false, "Bone::GetScaleIndex > Scale index is 0");
	}

	float Bone::GetScaleFactor(float lastTick, float nextTick, float animationTime)
	{
		float midWayLength = animationTime - lastTick;
		float framesDiff = nextTick - lastTick;
		return midWayLength / framesDiff;
	}
}