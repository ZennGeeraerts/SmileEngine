#include "smpch.h"
#include "mesh_filter.h"

namespace smile::renderer
{
    const DirectX::XMFLOAT4 MeshFilter::defaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
    const DirectX::XMFLOAT4 MeshFilter::defaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
    const DirectX::XMFLOAT3 MeshFilter::defaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
    const DirectX::XMFLOAT2 MeshFilter::defaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };
    const DirectX::XMFLOAT4 MeshFilter::defaultIndices4 = DirectX::XMFLOAT4{ -1, -1, -1, -1 };
}