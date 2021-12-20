#include "smpch.h"
#include "MeshLoader.h"

#include "SmileEngine/Core/Logger.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Smile
{
	std::vector<Ref<MeshFilter>> MeshLoader::LoadMesh(const std::string& filePath)
	{
		aiPropertyStore* pPropertyStore = aiCreatePropertyStore();
		aiSetImportPropertyInteger(pPropertyStore, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		
		const aiScene* pAiScene = aiImportFileExWithProperties(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality, nullptr, pPropertyStore);
		if (!pAiScene)
		{
			SM_LOG_WARNING("MeshLoader::LoadObj > Could not load file: %s: %s", filePath, aiGetErrorString());
			aiReleaseImport(pAiScene);
			aiReleasePropertyStore(pPropertyStore);
			return std::vector<Ref<MeshFilter>>{};
		}
		
		DirectX::XMMATRIX inverseGlobalTransformMat = DirectX::XMMATRIX{ 
									pAiScene->mRootNode->mTransformation.a1, pAiScene->mRootNode->mTransformation.b1, pAiScene->mRootNode->mTransformation.c1, pAiScene->mRootNode->mTransformation.d1,
									pAiScene->mRootNode->mTransformation.a2, pAiScene->mRootNode->mTransformation.b2, pAiScene->mRootNode->mTransformation.c2, pAiScene->mRootNode->mTransformation.d2,
									pAiScene->mRootNode->mTransformation.a3, pAiScene->mRootNode->mTransformation.b3, pAiScene->mRootNode->mTransformation.c3, pAiScene->mRootNode->mTransformation.d3,
									pAiScene->mRootNode->mTransformation.a4, pAiScene->mRootNode->mTransformation.b4, pAiScene->mRootNode->mTransformation.c4, pAiScene->mRootNode->mTransformation.d4 };
		inverseGlobalTransformMat = DirectX::XMMatrixInverse(nullptr, inverseGlobalTransformMat);
		DirectX::XMFLOAT4X4 inverseGlobalTransform{};
		DirectX::XMStoreFloat4x4(&inverseGlobalTransform, inverseGlobalTransformMat);

		std::vector<Ref<MeshFilter>> pMeshes{};
		pMeshes.resize(pAiScene->mNumMeshes);

		for (uint32_t m{}; m < pAiScene->mNumMeshes; ++m)
		{
			aiMesh* pAiMesh = pAiScene->mMeshes[m];

			pMeshes[m].reset(new MeshFilter{});
			pMeshes[m]->m_VertexCount = pAiMesh->mNumVertices;
			pMeshes[m]->m_Positions.resize(pAiMesh->mNumVertices);

			for (uint32_t v{}; v < pAiMesh->mNumVertices; ++v)
			{
				aiVector3D& vertex = pAiMesh->mVertices[v];
				if (&vertex)
				{
					pMeshes[m]->m_bUsePositions = true;
					pMeshes[m]->m_Positions[v] = *reinterpret_cast<DirectX::XMFLOAT3*>(&vertex);
				}

				aiVector3D& normal = pAiMesh->mNormals[v];
				if (&normal)
				{
					pMeshes[m]->m_bUseNormals = true;
					pMeshes[m]->m_Normals.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&normal));
				}

				aiVector3D& texCoord = pAiMesh->mTextureCoords[0][v];
				if (&texCoord)
				{
					pMeshes[m]->m_bUseTexCoords = true;
					pMeshes[m]->m_TexCoords.push_back({ texCoord.x, 1 - texCoord.y });
				}

				aiVector3D& tangent = pAiMesh->mTangents[v];
				if (&tangent)
				{
					pMeshes[m]->m_bUseTangents = true;
					pMeshes[m]->m_Tangents.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&tangent));
				}

				/*aiColor4D* pColor = pAiMesh->mColors[v];
				if (pColor)
				{
					pMeshes[m]->m_bUseColors = true;
					pMeshes[m]->m_Colors.push_back(*reinterpret_cast<DirectX::XMFLOAT4*>(pColor));
				}*/
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

			if (pAiMesh->HasBones())
			{
				pMeshes[m]->m_bUseBlendIndices = true;
				pMeshes[m]->m_bUseBlendWeights = true;

				pMeshes[m]->m_BlendIndices.resize(pAiMesh->mNumVertices);
				pMeshes[m]->m_BlendWeights.resize(pAiMesh->mNumVertices);
				std::fill(pMeshes[m]->m_BlendIndices.begin(), pMeshes[m]->m_BlendIndices.end(), DirectX::XMFLOAT4{ -1, -1, -1, -1 });

				LoadBones(pMeshes[m], pAiMesh, pAiScene);
			}

			pMeshes[m]->m_FilePath = filePath;
		}

		if (pAiScene->HasAnimations())
			LoadAnimations(pMeshes[0], pAiScene);

		aiReleaseImport(pAiScene);
		aiReleasePropertyStore(pPropertyStore);

		return pMeshes;
	}

	void MeshLoader::LoadBones(const Ref<MeshFilter>& pMesh, aiMesh* pAiMesh, const aiScene* pAiScene)
	{
		for (uint32_t i{}; i < pAiMesh->mNumBones; ++i)
		{
			aiBone* pBone = pAiMesh->mBones[i];
			if (pBone)
			{
				uint32_t boneID = -1;
				std::string boneName = pBone->mName.C_Str();

				if (pMesh->m_SkeletonMap.find(boneName) == pMesh->m_SkeletonMap.end())
				{
					BoneInfo boneInfo{};
					boneInfo.ID = pMesh->m_BoneCount;
					boneInfo.Offset = DirectX::XMFLOAT4X4{
									pBone->mOffsetMatrix.a1, pBone->mOffsetMatrix.b1, pBone->mOffsetMatrix.c1, pBone->mOffsetMatrix.d1,
									pBone->mOffsetMatrix.a2, pBone->mOffsetMatrix.b2, pBone->mOffsetMatrix.c2, pBone->mOffsetMatrix.d2,
									pBone->mOffsetMatrix.a3, pBone->mOffsetMatrix.b3, pBone->mOffsetMatrix.c3, pBone->mOffsetMatrix.d3,
									pBone->mOffsetMatrix.a4, pBone->mOffsetMatrix.b4, pBone->mOffsetMatrix.c4, pBone->mOffsetMatrix.d4 };

					pMesh->m_SkeletonMap[boneName] = boneInfo;
					boneID = pMesh->m_BoneCount;
					++pMesh->m_BoneCount;
				}
				else
				{
					boneID = pMesh->m_SkeletonMap[boneName].ID;
				}

				SM_ASSERT(boneID != -1, "MeshLoader::LoadBones > Invalid bone ID");

				for (uint32_t j{}; j < pBone->mNumWeights; ++j)
				{
					aiVertexWeight& weight = pBone->mWeights[j];
					if (&weight)
					{
						SM_ASSERT(weight.mVertexId <= pMesh->m_VertexCount, "MeshLoader::LoadBones > Invalid vertex ID");

						if (pMesh->m_BlendIndices[weight.mVertexId].x < 0)
						{
							pMesh->m_BlendIndices[weight.mVertexId].x = boneID;
							pMesh->m_BlendWeights[weight.mVertexId].x = weight.mWeight;
						}
						else if (pMesh->m_BlendIndices[weight.mVertexId].y < 0)
						{
							pMesh->m_BlendIndices[weight.mVertexId].y = boneID;
							pMesh->m_BlendWeights[weight.mVertexId].y = weight.mWeight;
						}
						else if (pMesh->m_BlendIndices[weight.mVertexId].z < 0)
						{
							pMesh->m_BlendIndices[weight.mVertexId].z = boneID;
							pMesh->m_BlendWeights[weight.mVertexId].z = weight.mWeight;
						}
						else if (pMesh->m_BlendIndices[weight.mVertexId].w < 0)
						{
							pMesh->m_BlendIndices[weight.mVertexId].w = boneID;
							pMesh->m_BlendWeights[weight.mVertexId].w = weight.mWeight;
						}
					}
				}
			}
		}
	}

	void MeshLoader::LoadAnimations(const Ref<MeshFilter>& pMesh, const aiScene* pAiScene)
	{
		pMesh->m_bHasAnimations = true;

		for (uint32_t i{}; i < pAiScene->mNumAnimations; ++i)
		{
			aiAnimation* pAnim = pAiScene->mAnimations[i];
			if (pAnim)
			{
				AnimationClip animClip{};
				animClip.Name = pAnim->mName.C_Str();
				animClip.Duration = static_cast<float>(pAnim->mDuration);
				animClip.TicksPerSecond = static_cast<float>(pAnim->mTicksPerSecond);
				
				LoadNodeHierarchy(animClip.RootNode, pAiScene->mRootNode);

				for (uint32_t j{}; j < pAnim->mNumChannels; ++j)
				{
					aiNodeAnim* pChannel = pAnim->mChannels[j];
					if (pChannel)
					{
						std::string boneName = pChannel->mNodeName.C_Str();
						auto boneInfoMapIt = pMesh->m_SkeletonMap.find(boneName);
						if (boneInfoMapIt != pMesh->m_SkeletonMap.end())
						{
							BoneInfo& boneInfo = (*boneInfoMapIt).second;
							Bone bone{ boneName, boneInfo.ID };

							bone.m_TranslationCount = pChannel->mNumPositionKeys;
							for (uint32_t k{}; k < pChannel->mNumPositionKeys; ++k)
							{
								KeyTranslation keyTranslation{};
								keyTranslation.Translation = *reinterpret_cast<DirectX::XMFLOAT3*>(&pChannel->mPositionKeys[k].mValue);
								keyTranslation.Tick = static_cast<float>(pChannel->mPositionKeys[k].mTime);
								bone.m_Translations.push_back(keyTranslation);
							}

							bone.m_RotationCount = pChannel->mNumRotationKeys;
							for (uint32_t k{}; k < pChannel->mNumRotationKeys; ++k)
							{
								KeyRotation keyRotation{};
								keyRotation.Rotation = DirectX::XMFLOAT4{ pChannel->mRotationKeys[k].mValue.x, pChannel->mRotationKeys[k].mValue.y, pChannel->mRotationKeys[k].mValue.z, pChannel->mRotationKeys[k].mValue.w };
								keyRotation.Tick = static_cast<float>(pChannel->mRotationKeys[k].mTime);
								bone.m_Rotations.push_back(keyRotation);
							}

							bone.m_ScaleCount = pChannel->mNumScalingKeys;
							for (uint32_t k{}; k < pChannel->mNumScalingKeys; ++k)
							{
								KeyScale keyScale{};
								keyScale.Scale = *reinterpret_cast<DirectX::XMFLOAT3*>(&pChannel->mScalingKeys[k].mValue);
								keyScale.Tick = static_cast<float>(pChannel->mScalingKeys[k].mTime);
								bone.m_Scales.push_back(keyScale);
							}

							animClip.Bones.push_back(bone);
						}
					}
				}

				pMesh->m_AnimationClips.push_back(animClip);
			}
		}
	}

	void MeshLoader::LoadNodeHierarchy(AnimationNode& dest, const aiNode* src)
	{
		dest.Name = src->mName.data;
		//if (src->mNumMeshes > 0)
		{
			dest.Transform = DirectX::XMFLOAT4X4{
							src->mTransformation.a1, src->mTransformation.b1, src->mTransformation.c1, src->mTransformation.d1,
							src->mTransformation.a2, src->mTransformation.b2, src->mTransformation.c2, src->mTransformation.d2,
							src->mTransformation.a3, src->mTransformation.b3, src->mTransformation.c3, src->mTransformation.d3,
							src->mTransformation.a4, src->mTransformation.b4, src->mTransformation.c4, src->mTransformation.d4 };
		}
		/*else
		{
			DirectX::XMStoreFloat4x4(&dest.Transform, DirectX::XMMatrixIdentity());
		}*/

		dest.ChildrenCount = src->mNumChildren;

		for (uint32_t i{}; i < src->mNumChildren; ++i)
		{
			AnimationNode newNode{};
			LoadNodeHierarchy(newNode, src->mChildren[i]);
			dest.Children.push_back(newNode);
		}
	}
}