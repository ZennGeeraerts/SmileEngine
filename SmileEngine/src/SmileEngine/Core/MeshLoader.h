#pragma once
#include "SmileEngine/Renderer/MeshFilter.h"

class aiMesh;
class aiScene;
class aiNode;

namespace Smile
{
	class MeshLoader final
	{
	public:
		MeshLoader() = default;

		std::vector<Ref<MeshFilter>> LoadMesh(const std::string& filePath);

	private:
		void LoadBones(const Ref<MeshFilter>& pMesh, aiMesh* pAiMesh, const aiScene* pAiScene);
		void LoadAnimations(const Ref<MeshFilter>& pMesh, const aiScene* pAiScene);
		void LoadNodeHierarchy(AnimationNode& dest, const aiNode* src);
	};
}

