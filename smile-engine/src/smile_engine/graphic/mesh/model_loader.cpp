/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "model_loader.h"

#include "model_loader_utils.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace smile::graphic
{
    Ref< graphic::Model > ModelLoader::LoadModel( const std::filesystem::path &filePath )
    {
        aiPropertyStore *pPropertyStore = aiCreatePropertyStore();
        aiSetImportPropertyInteger( pPropertyStore, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0 );

        const aiScene *pAiScene = aiImportFileExWithProperties( filePath.string().c_str(),
            aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_MakeLeftHanded,
            nullptr,
            pPropertyStore );
        if ( !pAiScene )
        {
            SM_LOG_WARNING(
                "ModelLoader::LoadModel > Could not load file: %s: %s", filePath.string(), aiGetErrorString() );
            aiReleaseImport( pAiScene );
            aiReleasePropertyStore( pPropertyStore );
            return nullptr;
        }

        Ref< graphic::Model > pModel = CreateRef< graphic::Model >();
        pModel->m_FilePath = filePath.string();

        if ( pAiScene->HasMeshes() )
        {
            for ( Uint32 m{}; m < pAiScene->mNumMeshes; ++m )
            {
                aiMesh *pAiMesh = pAiScene->mMeshes[m];

                if ( !pAiMesh->HasBones() )
                {
                    auto pMeshFilter = LoadMesh( pAiMesh );
                    pModel->m_pMeshes.push_back( pMeshFilter );
                }
                else
                {
                    auto pSkinnedMeshFilter = LoadSkinnedMesh( pAiMesh );
                    pModel->m_pSkinnedMeshes.push_back( pSkinnedMeshFilter );
                }
            }
        }

        if ( pAiScene->HasAnimations() )
        {
            for ( Uint32 i{}; i < pAiScene->mNumAnimations; ++i )
            {
                aiAnimation *pAiAnim = pAiScene->mAnimations[i];
                if ( pAiAnim )
                {
                    Ref< graphic::AnimationClip > pAnimClip = LoadAnimation( pModel, pAiAnim, pAiScene );
                    pModel->m_pAnimationClips.push_back( pAnimClip );
                }
            }
        }

        aiReleaseImport( pAiScene );
        aiReleasePropertyStore( pPropertyStore );

        return pModel;
    }

    Ref< graphic::MeshFilter > ModelLoader::LoadMesh( aiMesh *pAiMesh )
    {
        Ref< graphic::MeshFilter > pMeshFilter = CreateRef< graphic::MeshFilter >();
        pMeshFilter->m_Name = pAiMesh->mName.C_Str();

        LoadVertices( pMeshFilter, pAiMesh );

        return pMeshFilter;
    }

    Ref< graphic::SkinnedMeshFilter > ModelLoader::LoadSkinnedMesh( aiMesh *pAiMesh )
    {
        Ref< graphic::SkinnedMeshFilter > pSkinnedMeshFilter = CreateRef< graphic::SkinnedMeshFilter >();
        pSkinnedMeshFilter->m_Name = pAiMesh->mName.C_Str();
        pSkinnedMeshFilter->m_Semantics |= static_cast< Uint32 >( graphic::Semantic::BlendIndices );
        pSkinnedMeshFilter->m_Semantics |= static_cast< Uint32 >( graphic::Semantic::BlendWeights );

        pSkinnedMeshFilter->m_BlendIndices.resize( pAiMesh->mNumVertices );
        pSkinnedMeshFilter->m_BlendWeights.resize( pAiMesh->mNumVertices );
        std::fill( pSkinnedMeshFilter->m_BlendIndices.begin(),
            pSkinnedMeshFilter->m_BlendIndices.end(),
            DirectX::XMFLOAT4{ -1, -1, -1, -1 } );

        LoadVertices( pSkinnedMeshFilter, pAiMesh );
        LoadBones( pSkinnedMeshFilter, pAiMesh );

        return pSkinnedMeshFilter;
    }

    void ModelLoader::LoadVertices( const Ref< graphic::MeshFilter > &pMesh, aiMesh *pAiMesh )
    {
        pMesh->SetVertexCount( pAiMesh->mNumVertices );

        for ( Uint32 v{}; v < pAiMesh->mNumVertices; ++v )
        {
            aiVector3D &vertex = pAiMesh->mVertices[v];
            auto position = utils::ConvertToVector3( vertex );
            pMesh->AddPosition( position );

            if ( pAiMesh->HasNormals() )
            {
                aiVector3D &normal = pAiMesh->mNormals[v];
                pMesh->AddNormal( utils::ConvertToVector3( normal ) );
            }

            if ( pAiMesh->HasTextureCoords( 0 ) )
            {
                aiVector3D &texCoord = pAiMesh->mTextureCoords[0][v];
                pMesh->AddTexCoord( { texCoord.x, 1 - texCoord.y } );
            }

            if ( pAiMesh->HasTangentsAndBitangents() )
            {
                aiVector3D &tangent = pAiMesh->mTangents[v];
                pMesh->AddTangent( utils::ConvertToVector3( tangent ) );
            }

            if ( pAiMesh->HasVertexColors( 0 ) )
            {
                aiColor4D &color = pAiMesh->mColors[0][v];
                pMesh->AddColor( utils::ConvertToVector4( color ) );
            }
        }

        pMesh->SetIndexCount( pAiMesh->mNumFaces * 3 );

        for ( Uint32 f{}; f < pAiMesh->mNumFaces; ++f )
        {
            const Uint32 index0 = pAiMesh->mFaces[f].mIndices[0];
            const Uint32 index1 = pAiMesh->mFaces[f].mIndices[1];
            const Uint32 index2 = pAiMesh->mFaces[f].mIndices[2];

            const Uint32 indexIdx = static_cast< size_t >( f ) * 3;
            pMesh->AddIndex( indexIdx, index0 );
            pMesh->AddIndex( indexIdx + 1, index1 );
            pMesh->AddIndex( indexIdx + 2, index2 );
        }
    }

    void ModelLoader::LoadBones( const Ref< graphic::SkinnedMeshFilter > &pMesh, aiMesh *pAiMesh )
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
                    graphic::BoneInfo boneInfo{};
                    boneInfo.ID = pMesh->m_BoneCount;
                    boneInfo.Offset = DirectX::XMFLOAT4X4{ pBone->mOffsetMatrix.a1,
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

                    pMesh->m_SkeletonMap[boneName] = boneInfo;
                    boneID = pMesh->m_BoneCount;
                    ++pMesh->m_BoneCount;
                }
                else
                {
                    boneID = pMesh->m_SkeletonMap[boneName].ID;
                }

                SM_ASSERT( boneID != -1, "ModelLoader::LoadBones > Invalid bone ID" );

                for ( Uint32 j{}; j < pBone->mNumWeights; ++j )
                {
                    aiVertexWeight &weight = pBone->mWeights[j];
                    if ( &weight )
                    {
                        SM_ASSERT(
                            weight.mVertexId <= pMesh->m_VertexCount, "ModelLoader::LoadBones > Invalid vertex ID" );

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

    Ref< graphic::AnimationClip > ModelLoader::LoadAnimation( const Ref< graphic::Model > &pModel,
        const aiAnimation *pAiAnim,
        const aiScene *pAiScene )
    {
        Ref< graphic::AnimationClip > pAnimClip = CreateRef< graphic::AnimationClip >();
        pAnimClip->Name = pAiAnim->mName.C_Str();
        pAnimClip->Duration = static_cast< float >( pAiAnim->mDuration );
        pAnimClip->TicksPerSecond = static_cast< float >( pAiAnim->mTicksPerSecond );

        LoadNodeHierarchy( pAnimClip->RootNode, pAiScene->mRootNode );

        for ( Uint32 j{}; j < pAiAnim->mNumChannels; ++j )
        {
            aiNodeAnim *pChannel = pAiAnim->mChannels[j];
            if ( pChannel )
            {
                std::string boneName = pChannel->mNodeName.C_Str();

                for ( auto pSkinnedMesh : pModel->m_pSkinnedMeshes )
                {
                    auto boneInfoMapIt = pSkinnedMesh->m_SkeletonMap.find( boneName );
                    if ( boneInfoMapIt != pSkinnedMesh->m_SkeletonMap.end() )
                    {
                        graphic::BoneInfo &boneInfo = ( *boneInfoMapIt ).second;
                        graphic::Bone bone{ boneName, boneInfo.ID };

                        bone.m_TranslationCount = pChannel->mNumPositionKeys;
                        for ( Uint32 k{}; k < pChannel->mNumPositionKeys; ++k )
                        {
                            graphic::KeyTranslation keyTranslation{};
                            keyTranslation.Translation =
                                *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mPositionKeys[k].mValue );
                            keyTranslation.Tick = static_cast< float >( pChannel->mPositionKeys[k].mTime );
                            bone.m_Translations.push_back( keyTranslation );
                        }

                        bone.m_RotationCount = pChannel->mNumRotationKeys;
                        for ( Uint32 k{}; k < pChannel->mNumRotationKeys; ++k )
                        {
                            graphic::KeyRotation keyRotation{};
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
                            graphic::KeyScale keyScale{};
                            keyScale.Scale =
                                *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mScalingKeys[k].mValue );
                            keyScale.Tick = static_cast< float >( pChannel->mScalingKeys[k].mTime );
                            bone.m_Scales.push_back( keyScale );
                        }

                        pAnimClip->Bones.push_back( bone );
                        break;
                    }
                }
            }
        }

        return pAnimClip;
    }

    void ModelLoader::LoadNodeHierarchy( graphic::AnimationNode &dest, const aiNode *pSrc )
    {
        dest.Name = pSrc->mName.data;
        // if (src->mNumMeshes > 0)
        {
            dest.Transform = DirectX::XMFLOAT4X4{ pSrc->mTransformation.a1,
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

        dest.ChildrenCount = pSrc->mNumChildren;

        for ( Uint32 i{}; i < pSrc->mNumChildren; ++i )
        {
            graphic::AnimationNode newNode{};
            LoadNodeHierarchy( newNode, pSrc->mChildren[i] );
            dest.Children.push_back( newNode );
        }
    }
}