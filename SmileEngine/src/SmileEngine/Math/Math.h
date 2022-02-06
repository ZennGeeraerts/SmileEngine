#pragma once

namespace Smile::Math
{
	// Vector
	struct Vector2 final
	{
		union
		{
			float M[2];
			struct
			{
				float x;
				float y;
			};
		};
	};

	struct Vector3 final
	{
		union
		{
			float M[3];
			struct
			{
				float x;
				float y;
				float z;
			};
		};
	};

	struct Vector4 final
	{
		union
		{
			float M[4];
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};
	};

	// Matrix
	struct Matrix4x4 final
	{
		union
		{
			float M[4][4];
			struct
			{
				float M00, M01, M02, M03;
				float M10, M11, M12, M13;
				float M20, M21, M22, M23;
				float M30, M31, M32, M33;
			};
		};
	};

	struct Matrix4x3 final
	{
		union
		{
			float M[4][3];
			struct
			{
				float M00, M01, M02;
				float M10, M11, M12;
				float M20, M21, M22;
				float M30, M31, M32;
			};
		};
	};

	struct Matrix4x1 final
	{
		union
		{
			float M[4];
			struct
			{
				float M00, M01, M02, M03;
			};
		};
	};

	DirectX::XMFLOAT3 QuaternionToEuler(const DirectX::XMFLOAT4& quaternion);
	bool DecomposeMatrix(const DirectX::XMFLOAT4X4& transform, DirectX::XMFLOAT3& translation, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale);
}