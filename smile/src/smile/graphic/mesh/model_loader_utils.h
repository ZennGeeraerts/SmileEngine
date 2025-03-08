/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>

namespace smile::graphic::utils
{
    DirectX::XMFLOAT3 ConvertToVector3( const aiVector3D &aiVector );
    DirectX::XMFLOAT4 ConvertToVector4( const aiColor4D &aiColor );
}