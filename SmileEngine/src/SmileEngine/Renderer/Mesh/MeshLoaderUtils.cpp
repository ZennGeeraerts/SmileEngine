#include "smpch.h"
#include "MeshLoaderUtils.h"

namespace smile::utils
{
    DirectX::XMFLOAT3 ConvertToVector3( const aiVector3D &aiVector )
    {
        return *( DirectX::XMFLOAT3 * )( &aiVector );
    }

    DirectX::XMFLOAT4 ConvertToVector4( const aiColor4D &aiColor )
    {
        return *( DirectX::XMFLOAT4 * )( &aiColor );
    }
}