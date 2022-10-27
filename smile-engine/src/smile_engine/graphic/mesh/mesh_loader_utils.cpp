#include "smpch.h"
#include "mesh_loader_utils.h"

namespace smile::graphic::utils
{
    DirectX::XMFLOAT3 convertToVector3( const aiVector3D &ai_vector )
    {
        return *( DirectX::XMFLOAT3 * )( &ai_vector );
    }

    DirectX::XMFLOAT4 convertToVector4( const aiColor4D &ai_color )
    {
        return *( DirectX::XMFLOAT4 * )( &ai_color );
    }
}