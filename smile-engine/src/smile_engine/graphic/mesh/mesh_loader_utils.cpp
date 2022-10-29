#include "smpch.h"
#include "mesh_loader_utils.h"

namespace Smile::Graphic::Utils
{
    DirectX::XMFLOAT3 convertToVector3( const aiVector3D &aiVector )
    {
        return *( DirectX::XMFLOAT3 * )( &aiVector );
    }

    DirectX::XMFLOAT4 convertToVector4( const aiColor4D &aiColor )
    {
        return *( DirectX::XMFLOAT4 * )( &aiColor );
    }
}