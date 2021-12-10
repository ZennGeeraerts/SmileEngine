#pragma once
#include "SmileEngine/Renderer/Mesh.h"

class aiMesh;
class aiScene;
class aiNode;

namespace Smile
{
	class MeshLoader final
	{
	public:
		MeshLoader() = default;

		std::vector<Ref<Mesh>> LoadMesh(const std::string& filePath);

	private:
		void LoadBones(const Ref<Mesh>& pMesh, aiMesh* pAiMesh, const aiScene* pAiScene);
		void LoadAnimations(const Ref<Mesh>& pMesh, const aiScene* pAiScene);
		void LoadNodeHierarchy(AnimationNode& dest, const aiNode* src);
	};
}

