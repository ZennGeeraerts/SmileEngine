#pragma once

namespace Smile::Math
{
	DirectX::XMFLOAT3 QuaternionToEuler(const DirectX::XMFLOAT4& quaternion);
	bool DecomposeMatrix(const DirectX::XMFLOAT4X4& transform, DirectX::XMFLOAT3& translation, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale);
}
