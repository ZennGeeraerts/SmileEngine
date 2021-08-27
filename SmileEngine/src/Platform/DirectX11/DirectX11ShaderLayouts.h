#pragma once
#include "smpch.h"

namespace Smile
{
	class ShaderLayouts
	{
	public:
		static D3D11_INPUT_ELEMENT_DESC m_PosLayout[];
		static D3D11_INPUT_ELEMENT_DESC m_PosColLayout[];
	};

	D3D11_INPUT_ELEMENT_DESC ShaderLayouts::m_PosLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC ShaderLayouts::m_PosColLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
}
