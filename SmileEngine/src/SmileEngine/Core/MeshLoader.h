#pragma once
#include "SmileEngine/Renderer/StaticMeshFilter.h"
#include "SmileEngine/Renderer/SkinnedMeshFilter.h"

class aiMesh;
class aiScene;
class aiNode;

namespace Smile
{
	class MeshLoader final
	{
	public:
		static std::vector<Ref<StaticMeshFilter>> LoadStaticMesh(const std::string& filePath);
		static std::vector<Ref<SkinnedMeshFilter>> LoadSkinnedMesh(const std::string& filePath);

	private:
		static void LoadVertices(const Ref<StaticMeshFilter>& pMesh, aiMesh* pAiMesh);
		static void LoadBones(const Ref<SkinnedMeshFilter>& pMesh, aiMesh* pAiMesh, const aiScene* pAiScene);
		static void LoadAnimations(const Ref<SkinnedMeshFilter>& pMesh, const aiScene* pAiScene);
		static void LoadNodeHierarchy(AnimationNode& dest, const aiNode* src);
	};
}

