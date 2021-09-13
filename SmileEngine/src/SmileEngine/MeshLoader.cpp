#include "smpch.h"
#include "MeshLoader.h"

#include "SmileEngine/Logger.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Smile
{
	std::vector<Ref<Mesh>> MeshLoader::LoadMesh(const std::string& filePath)
	{
		const aiScene* pScene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (!pScene)
		{
			SM_LOG_WARNING("MeshLoader::LoadObj > Could not load file: %s: %s", filePath, aiGetErrorString());
			aiReleaseImport(pScene);
			return std::vector<Ref<Mesh>>{};
		}

		std::vector<Ref<Mesh>> pMeshes{};
		pMeshes.resize(pScene->mNumMeshes);

		for (uint32_t m{}; m < pScene->mNumMeshes; ++m)
		{
			aiMesh* pAiMesh = pScene->mMeshes[m];

			pMeshes[m].reset(new Mesh{});
			pMeshes[m]->m_VertexCount = pAiMesh->mNumVertices;
			pMeshes[m]->m_Positions.resize(pAiMesh->mNumVertices);

			for (uint32_t v{}; v < pAiMesh->mNumVertices; ++v)
			{
				aiVector3D& vertex = pAiMesh->mVertices[v];
				if (&vertex)
				{
					pMeshes[m]->bUsePositions = true;
					pMeshes[m]->m_Positions[v] = *reinterpret_cast<DirectX::XMFLOAT3*>(&vertex);
				}

				aiVector3D& normal = pAiMesh->mNormals[v];
				if (&normal)
				{
					pMeshes[m]->bUseNormals = true;
					pMeshes[m]->m_Normals.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&normal));
				}

				aiVector3D& texCoord = pAiMesh->mTextureCoords[0][v];
				if (&texCoord)
				{
					pMeshes[m]->bUseTexCoords = true;
					pMeshes[m]->m_TexCoords.push_back({ texCoord.x, 1 - texCoord.y });
				}

				aiVector3D& tangent = pAiMesh->mTangents[v];
				if (&tangent)
				{
					pMeshes[m]->bUseTangents = true;
					pMeshes[m]->m_Tangents.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&tangent));
				}
			}

			pMeshes[m]->m_Indices.resize(static_cast<size_t>(pAiMesh->mNumFaces) * 3);

			for (uint32_t f{}; f < pAiMesh->mNumFaces; ++f)
			{
				uint32_t index0 = pAiMesh->mFaces[f].mIndices[0];
				uint32_t index1 = pAiMesh->mFaces[f].mIndices[1];
				uint32_t index2 = pAiMesh->mFaces[f].mIndices[2];

				uint32_t indexIdx = static_cast<size_t>(f) * 3;
				pMeshes[m]->m_Indices[indexIdx] = index0;
				pMeshes[m]->m_Indices[static_cast<size_t>(indexIdx) + 1] = index1;
				pMeshes[m]->m_Indices[static_cast<size_t>(indexIdx) + 2] = index2;
			}
		}

		aiReleaseImport(pScene);

		return pMeshes;
	}
}