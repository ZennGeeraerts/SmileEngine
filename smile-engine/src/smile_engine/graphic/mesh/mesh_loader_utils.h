#pragma once

#include <DirectXMath.h>
#include <assimp/vector3.h>
#include <assimp/color4.h>

namespace Smile::Graphic::Utils
{
    DirectX::XMFLOAT3 convertToVector3( const aiVector3D &aiVector );
    DirectX::XMFLOAT4 convertToVector4( const aiColor4D &aiColor );
}