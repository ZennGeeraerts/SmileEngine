/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace geometric
{
	struct Box
	{
        DirectX::XMFLOAT3 Origin = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
	};
}