#include "smpch.h"
#include "Bone.h"
#include "SmileEngine/Core/Logger.h"

namespace Smile
{
	Bone::Bone(const std::string& name, uint32_t id)
		: m_Name{ name }
		, m_ID{ id }
	{

	}

	void Bone::OnUpdate(float animationTime)
	{
		DirectX::XMFLOAT4X4 translation{};
		InterpolateTranslation(translation, animationTime);
		DirectX::XMFLOAT4X4 rotation{};
		InterpolateRotation(rotation, animationTime);
		DirectX::XMFLOAT4X4 scale{};
		InterpolateScale(scale, animationTime);

		DirectX::XMMATRIX localTransformMat = DirectX::XMLoadFloat4x4(&scale) * DirectX::XMLoadFloat4x4(&rotation) * DirectX::XMLoadFloat4x4(&translation);
		/*DirectX::XMMATRIX rotYMat = DirectX::XMMatrixRotationY(DirectX::XM_PIDIV2);
		DirectX::XMMATRIX rotZMat = DirectX::XMMatrixRotationZ(DirectX::XM_PIDIV2);
		localTransformMat = localTransformMat * rotZMat * rotYMat;*/
		DirectX::XMStoreFloat4x4(&m_LocalTransform, localTransformMat);
	}

	void Bone::InterpolateTranslation(DirectX::XMFLOAT4X4& translation, float animationTime)
	{
		if (m_TranslationCount == 1)
		{
			DirectX::XMMATRIX translationMat = DirectX::XMMatrixTranslation(m_Translations[0].Translation.x, m_Translations[0].Translation.y, m_Translations[0].Translation.z);
			DirectX::XMStoreFloat4x4(&translation, translationMat);
			return;
		}

		uint32_t p0Index = GetTranslationIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Translations[p0Index].Tick, m_Translations[p1Index].Tick, animationTime);

		DirectX::XMVECTOR finalTranslationVec = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&m_Translations[p0Index].Translation), DirectX::XMLoadFloat3(&m_Translations[p1Index].Translation), scaleFactor);
		DirectX::XMMATRIX finalTranslationMat = DirectX::XMMatrixTranslationFromVector(finalTranslationVec);
		DirectX::XMStoreFloat4x4(&translation, finalTranslationMat);
	}

	uint32_t Bone::GetTranslationIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_TranslationCount - 1); ++i)
		{
			if (animationTime < m_Translations[i + 1].Tick)
				return i;
		}
		
		SM_ASSERT(false, "Bone::GetTranslationIndex > Translation index is 0");
	}

	void Bone::InterpolateRotation(DirectX::XMFLOAT4X4& rotation, float animationTime)
	{
		if (m_TranslationCount == 1)
		{
			DirectX::XMVECTOR rotationVec = DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&m_Rotations[0].Rotation));
			DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationQuaternion(rotationVec);
			DirectX::XMStoreFloat4x4(&rotation, rotationMat);
			return;
		}

		uint32_t p0Index = GetRotationIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Rotations[p0Index].Tick, m_Rotations[p1Index].Tick, animationTime);

		DirectX::XMVECTOR finalRotationVec = DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&m_Rotations[p0Index].Rotation), DirectX::XMLoadFloat4(&m_Rotations[p1Index].Rotation), scaleFactor);
		finalRotationVec = DirectX::XMQuaternionNormalize(finalRotationVec);
		DirectX::XMMATRIX finalRotationMat = DirectX::XMMatrixRotationQuaternion(finalRotationVec);
		DirectX::XMStoreFloat4x4(&rotation, finalRotationMat);
	}

	uint32_t Bone::GetRotationIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_RotationCount - 1); ++i)
		{
			if (animationTime < m_Rotations[i + 1].Tick)
				return i;
		}

		SM_ASSERT(false, "Bone::GetRotationIndex > Rotation index is 0");
	}

	void Bone::InterpolateScale(DirectX::XMFLOAT4X4& scale, float animationTime)
	{
		if (m_ScaleCount == 1)
		{
			DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(m_Scales[0].Scale.x, m_Scales[0].Scale.y, m_Scales[0].Scale.z);
			DirectX::XMStoreFloat4x4(&scale, scaleMat);
			return;
		}

		uint32_t p0Index = GetScaleIndex(animationTime);
		uint32_t p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Scales[p0Index].Tick, m_Scales[p1Index].Tick, animationTime);
		DirectX::XMVECTOR finalScaleVec = DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&m_Scales[p0Index].Scale), DirectX::XMLoadFloat3(&m_Scales[p1Index].Scale), scaleFactor);
		DirectX::XMMATRIX finalScaleMat = DirectX::XMMatrixTranslationFromVector(finalScaleVec);
		DirectX::XMStoreFloat4x4(&scale, finalScaleMat);
	}

	uint32_t Bone::GetScaleIndex(float animationTime)
	{
		for (uint32_t i{}; i < (m_ScaleCount - 1); ++i)
		{
			if (animationTime < m_Scales[i + 1].Tick)
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