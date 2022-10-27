#pragma once

#include <DirectXMath.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>

namespace smile::graphic::utils
{
    DirectX::XMFLOAT3 convertToVector3( const aiVector3D &ai_vector );
    DirectX::XMFLOAT4 convertToVector4( const aiColor4D &ai_color );
}