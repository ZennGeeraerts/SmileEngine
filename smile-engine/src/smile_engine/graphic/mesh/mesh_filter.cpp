#include "smpch.h"
#include "mesh_filter.h"

namespace Smile::Graphic
{
    const DirectX::XMFLOAT4 MeshFilter::s_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
    const DirectX::XMFLOAT4 MeshFilter::s_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
    const DirectX::XMFLOAT3 MeshFilter::s_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
    const DirectX::XMFLOAT2 MeshFilter::s_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };
    const DirectX::XMFLOAT4 MeshFilter::s_DefaultIndices4 = DirectX::XMFLOAT4{ -1, -1, -1, -1 };
}