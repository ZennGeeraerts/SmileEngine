#pragma once

namespace Smile
{
	class ShaderLayouts
	{
	public:
		static D3D11_INPUT_ELEMENT_DESC m_PosLayout[];
		static D3D11_INPUT_ELEMENT_DESC m_PosColLayout[];
	};

	struct PosVertex
	{
		DirectX::XMFLOAT3 Position;
	};

	struct PosColVertex
	{
		PosColVertex(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& col)
			: Position{ pos }
			, Color{ col }
		{
		}

		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
	};
}
