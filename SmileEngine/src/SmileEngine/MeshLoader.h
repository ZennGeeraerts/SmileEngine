#pragma once
#include "Renderer/Mesh.h"

namespace Smile
{
	class MeshLoader final
	{
	public:
		MeshLoader() = default;

		std::vector<Ref<Mesh>> LoadMesh(const std::string& filePath);
	};
}

