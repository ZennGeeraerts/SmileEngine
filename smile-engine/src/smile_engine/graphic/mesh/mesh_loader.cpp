#include "smpch.h"
#include "mesh_loader.h"

#include "smile_engine/core/logger.h"
#include "mesh_loader_utils.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace smile::graphic
{
    std::vector< Ref< StaticMeshFilter > > MeshLoader::LoadStaticMesh( const std::string &filePath )
    {
        aiPropertyStore *pPropertyStore = aiCreatePropertyStore();
        aiSetImportPropertyInteger( pPropertyStore, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0 );

        const aiScene *pAiScene = aiImportFileExWithProperties(
            filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality, nullptr, pPropertyStore );
        if ( !pAiScene )
        {
            SM_LOG_WARNING( "MeshLoader::loadStaticMesh > Could not load file: %s: %s", filePath, aiGetErrorString() );
            aiReleaseImport( pAiScene );
            aiReleasePropertyStore( pPropertyStore );
            return std::vector< Ref< StaticMeshFilter > >{};
        }

        std::vector< Ref< StaticMeshFilter > > pStaticMeshes{};
        pStaticMeshes.resize( pAiScene->mNumMeshes );

        for ( Uint32 m{}; m < pAiScene->mNumMeshes; ++m )
        {
            aiMesh *pAiMesh = pAiScene->mMeshes[m];
            pStaticMeshes[m].reset( new StaticMeshFilter{} );

            LoadVertices( pStaticMeshes[m], pAiMesh );
            pStaticMeshes[m]->m_FilePath = filePath;
        }

        aiReleaseImport( pAiScene );
        aiReleasePropertyStore( pPropertyStore );

        return pStaticMeshes;
    }

    std::vector< Ref< SkinnedMeshFilter > > MeshLoader::LoadSkinnedMesh( const std::string &filePath )
    {
        aiPropertyStore *property_store = aiCreatePropertyStore();
        aiSetImportPropertyInteger( property_store, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0 );

        const aiScene *pAiScene = aiImportFileExWithProperties(
            filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality, nullptr, property_store );
        if ( !pAiScene )
        {
            SM_LOG_WARNING( "MeshLoader::LoadSkinnedMesh > Could not load file: %s: %s", filePath, aiGetErrorString() );
            aiReleaseImport( pAiScene );
            aiReleasePropertyStore( property_store );
            return std::vector< Ref< SkinnedMeshFilter > >{};
        }

        /*DirectX::XMMATRIX inverseGlobalTransformMat = DirectX::XMMATRIX{
                                    pAiScene->mRootNode->mTransformation.a1, pAiScene->mRootNode->mTransformation.b1,
        pAiScene->mRootNode->mTransformation.c1, pAiScene->mRootNode->mTransformation.d1,
                                    pAiScene->mRootNode->mTransformation.a2, pAiScene->mRootNode->mTransformation.b2,
        pAiScene->mRootNode->mTransformation.c2, pAiScene->mRootNode->mTransformation.d2,
                                    pAiScene->mRootNode->mTransformation.a3, pAiScene->mRootNode->mTransformation.b3,
        pAiScene->mRootNode->mTransformation.c3, pAiScene->mRootNode->mTransformation.d3,
                                    pAiScene->mRootNode->mTransformation.a4, pAiScene->mRootNode->mTransformation.b4,
        pAiScene->mRootNode->mTransformation.c4, pAiScene->mRootNode->mTransformation.d4 }; inverseGlobalTransformMat =
        DirectX::XMMatrixInverse(nullptr, inverseGlobalTransformMat); DirectX::XMFLOAT4X4 inverseGlobalTransform{};
        DirectX::XMStoreFloat4x4(&inverseGlobalTransform, inverseGlobalTransformMat);*/

        std::vector< Ref< SkinnedMeshFilter > > pSkinnedMeshes{};
        pSkinnedMeshes.resize( pAiScene->mNumMeshes );

        for ( Uint32 m{}; m < pAiScene->mNumMeshes; ++m )
        {
            aiMesh *pAiMesh = pAiScene->mMeshes[m];
            pSkinnedMeshes[m].reset( new SkinnedMeshFilter{} );

            LoadVertices( pSkinnedMeshes[m], pAiMesh );

            if ( pAiMesh->HasBones() )
            {
                pSkinnedMeshes[m]->m_UseBlendIndices = true;
                pSkinnedMeshes[m]->m_UseBlendWeights = true;

                pSkinnedMeshes[m]->m_BlendIndices.resize( pAiMesh->mNumVertices );
                pSkinnedMeshes[m]->m_BlendWeights.resize( pAiMesh->mNumVertices );
                std::fill( pSkinnedMeshes[m]->m_BlendIndices.begin(),
                    pSkinnedMeshes[m]->m_BlendIndices.end(),
                    DirectX::XMFLOAT4{ -1, -1, -1, -1 } );

                LoadBones( pSkinnedMeshes[m], pAiMesh, pAiScene );
            }

            pSkinnedMeshes[m]->m_FilePath = filePath;
        }

        if ( pAiScene->HasAnimations() )
            LoadAnimations( pSkinnedMeshes[0], pAiScene );

        aiReleaseImport( pAiScene );
        aiReleasePropertyStore( property_store );

        return pSkinnedMeshes;
    }

    void MeshLoader::LoadVertices( const Ref< MeshFilter > &pMesh, aiMesh *pAiMesh )
    {
        pMesh->SetVertexCount( pAiMesh->mNumVertices );

        for ( Uint32 v{}; v < pAiMesh->mNumVertices; ++v )
        {
            aiVector3D &vertex = pAiMesh->mVertices[v];
            pMesh->AddPosition( utils::convertToVector3( vertex ) );

            if ( pAiMesh->HasNormals() )
            {
                aiVector3D &normal = pAiMesh->mNormals[v];
                pMesh->AddNormal( utils::convertToVector3( normal ) );
            }

            if ( pAiMesh->HasTextureCoords( 0 ) )
            {
                aiVector3D &texCoord = pAiMesh->mTextureCoords[0][v];
                pMesh->AddTexCoord( { texCoord.x, 1 - texCoord.y } );
            }

            if ( pAiMesh->HasTangentsAndBitangents() )
            {
                aiVector3D &tangent = pAiMesh->mTangents[v];
                pMesh->AddTangent( utils::convertToVector3( tangent ) );
            }

            if ( pAiMesh->HasVertexColors( 0 ) )
            {
                aiColor4D &color = pAiMesh->mColors[0][v];
                pMesh->AddColor( utils::convertToVector4( color ) );
            }
        }

        pMesh->SetIndexCount( pAiMesh->mNumFaces * 3 );

        for ( Uint32 f{}; f < pAiMesh->mNumFaces; ++f )
        {
            Uint32 index0 = pAiMesh->mFaces[f].mIndices[0];
            Uint32 index1 = pAiMesh->mFaces[f].mIndices[1];
            Uint32 index2 = pAiMesh->mFaces[f].mIndices[2];

            Uint32 indexIdx = static_cast< size_t >( f ) * 3;
            pMesh->AddIndex( indexIdx, index0 );
            pMesh->AddIndex( indexIdx + 1, index1 );
            pMesh->AddIndex( indexIdx + 2, index2 );
        }
    }

    void MeshLoader::LoadBones( const Ref< SkinnedMeshFilter > &pMesh, aiMesh *pAiMesh, const aiScene *pAiScene )
    {
        for ( Uint32 i{}; i < pAiMesh->mNumBones; ++i )
        {
            aiBone *pBone = pAiMesh->mBones[i];
            if ( pBone )
            {
                Uint32 boneID = -1;
                std::string boneName = pBone->mName.C_Str();

                if ( pMesh->m_SkeletonMap.find( boneName ) == pMesh->m_SkeletonMap.end() )
                {
                    BoneInfo bone_info{};
                    bone_info.id = pMesh->m_BoneCount;
                    bone_info.offset = DirectX::XMFLOAT4X4{ pBone->mOffsetMatrix.a1,
                        pBone->mOffsetMatrix.b1,
                        pBone->mOffsetMatrix.c1,
                        pBone->mOffsetMatrix.d1,
                        pBone->mOffsetMatrix.a2,
                        pBone->mOffsetMatrix.b2,
                        pBone->mOffsetMatrix.c2,
                        pBone->mOffsetMatrix.d2,
                        pBone->mOffsetMatrix.a3,
                        pBone->mOffsetMatrix.b3,
                        pBone->mOffsetMatrix.c3,
                        pBone->mOffsetMatrix.d3,
                        pBone->mOffsetMatrix.a4,
                        pBone->mOffsetMatrix.b4,
                        pBone->mOffsetMatrix.c4,
                        pBone->mOffsetMatrix.d4 };

                    pMesh->m_SkeletonMap[boneName] = bone_info;
                    boneID = pMesh->m_BoneCount;
                    ++pMesh->m_BoneCount;
                }
                else
                {
                    boneID = pMesh->m_SkeletonMap[boneName].id;
                }

                SM_ASSERT( boneID != -1, "MeshLoader::loadBones > Invalid bone ID" );

                for ( Uint32 j{}; j < pBone->mNumWeights; ++j )
                {
                    aiVertexWeight &weight = pBone->mWeights[j];
                    if ( &weight )
                    {
                        SM_ASSERT(
                            weight.mVertexId <= pMesh->m_VertexCount, "MeshLoader::loadBones > Invalid vertex ID" );

                        if ( pMesh->m_BlendIndices[weight.mVertexId].x < 0 )
                        {
                            pMesh->m_BlendIndices[weight.mVertexId].x = static_cast< float >( boneID );
                            pMesh->m_BlendWeights[weight.mVertexId].x = weight.mWeight;
                        }
                        else if ( pMesh->m_BlendIndices[weight.mVertexId].y < 0 )
                        {
                            pMesh->m_BlendIndices[weight.mVertexId].y = static_cast< float >( boneID );
                            pMesh->m_BlendWeights[weight.mVertexId].y = weight.mWeight;
                        }
                        else if ( pMesh->m_BlendIndices[weight.mVertexId].z < 0 )
                        {
                            pMesh->m_BlendIndices[weight.mVertexId].z = static_cast< float >( boneID );
                            pMesh->m_BlendWeights[weight.mVertexId].z = weight.mWeight;
                        }
                        else if ( pMesh->m_BlendIndices[weight.mVertexId].w < 0 )
                        {
                            pMesh->m_BlendIndices[weight.mVertexId].w = static_cast< float >( boneID );
                            pMesh->m_BlendWeights[weight.mVertexId].w = weight.mWeight;
                        }
                    }
                }
            }
        }
    }

    void MeshLoader::LoadAnimations( const Ref< SkinnedMeshFilter > &pMesh, const aiScene *pAiScene )
    {
        pMesh->m_IsAnimated = true;

        for ( Uint32 i{}; i < pAiScene->mNumAnimations; ++i )
        {
            aiAnimation *pAnim = pAiScene->mAnimations[i];
            if ( pAnim )
            {
                AnimationClip animClip{};
                animClip.name = pAnim->mName.C_Str();
                animClip.duration = static_cast< float >( pAnim->mDuration );
                animClip.ticksPerSecond = static_cast< float >( pAnim->mTicksPerSecond );

                LoadNodeHierarchy( animClip.rootNode, pAiScene->mRootNode );

                for ( Uint32 j{}; j < pAnim->mNumChannels; ++j )
                {
                    aiNodeAnim *pChannel = pAnim->mChannels[j];
                    if ( pChannel )
                    {
                        std::string bone_name = pChannel->mNodeName.C_Str();
                        auto boneInfoMapIt = pMesh->m_SkeletonMap.find( bone_name );
                        if ( boneInfoMapIt != pMesh->m_SkeletonMap.end() )
                        {
                            BoneInfo &boneInfo = ( *boneInfoMapIt ).second;
                            Bone bone{ bone_name, boneInfo.id };

                            bone.m_TranslationCount = pChannel->mNumPositionKeys;
                            for ( Uint32 k{}; k < pChannel->mNumPositionKeys; ++k )
                            {
                                KeyTranslation keyTranslation{};
                                keyTranslation.Translation =
                                    *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mPositionKeys[k].mValue );
                                keyTranslation.Tick = static_cast< float >( pChannel->mPositionKeys[k].mTime );
                                bone.m_Translations.push_back( keyTranslation );
                            }

                            bone.m_RotationCount = pChannel->mNumRotationKeys;
                            for ( Uint32 k{}; k < pChannel->mNumRotationKeys; ++k )
                            {
                                KeyRotation keyRotation{};
                                keyRotation.Rotation = DirectX::XMFLOAT4{ pChannel->mRotationKeys[k].mValue.x,
                                    pChannel->mRotationKeys[k].mValue.y,
                                    pChannel->mRotationKeys[k].mValue.z,
                                    pChannel->mRotationKeys[k].mValue.w };
                                keyRotation.Tick = static_cast< float >( pChannel->mRotationKeys[k].mTime );
                                bone.m_Rotations.push_back( keyRotation );
                            }

                            bone.m_ScaleCount = pChannel->mNumScalingKeys;
                            for ( Uint32 k{}; k < pChannel->mNumScalingKeys; ++k )
                            {
                                KeyScale keyScale{};
                                keyScale.Scale =
                                    *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mScalingKeys[k].mValue );
                                keyScale.Tick = static_cast< float >( pChannel->mScalingKeys[k].mTime );
                                bone.m_Scales.push_back( keyScale );
                            }

                            animClip.bones.push_back( bone );
                        }
                    }
                }

                pMesh->m_AnimationClips.push_back( animClip );
            }
        }
    }

    void MeshLoader::LoadNodeHierarchy( AnimationNode &dest, const aiNode *pSrc )
    {
        dest.name = pSrc->mName.data;
        // if (src->mNumMeshes > 0)
        {
            dest.transform = DirectX::XMFLOAT4X4{ 
                pSrc->mTransformation.a1,
                pSrc->mTransformation.b1,
                pSrc->mTransformation.c1,
                pSrc->mTransformation.d1,
                pSrc->mTransformation.a2,
                pSrc->mTransformation.b2,
                pSrc->mTransformation.c2,
                pSrc->mTransformation.d2,
                pSrc->mTransformation.a3,
                pSrc->mTransformation.b3,
                pSrc->mTransformation.c3,
                pSrc->mTransformation.d3,
                pSrc->mTransformation.a4,
                pSrc->mTransformation.b4,
                pSrc->mTransformation.c4,
                pSrc->mTransformation.d4 };
        }
        /*else
        {
            DirectX::XMStoreFloat4x4(&dest.Transform, DirectX::XMMatrixIdentity());
        }*/

        dest.childrenCount = pSrc->mNumChildren;

        for ( Uint32 i{}; i < pSrc->mNumChildren; ++i )
        {
            AnimationNode newNode{};
            LoadNodeHierarchy( newNode, pSrc->mChildren[i] );
            dest.children.push_back( newNode );
        }
    }
}