/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "model_loader.h"

#include "model_loader_utils.h"
#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace smile::graphic
{
    ModelLoader::ModelLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::Asset > ModelLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadModel( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    Model::Ref ModelLoader::LoadModel( const std::filesystem::path &filePath ) const
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
                "ModelLoader::LoadModel > Could not load file: {0}: {1}", filePath.string(), aiGetErrorString() );
            aiReleaseImport( pAiScene );
            aiReleasePropertyStore( pPropertyStore );
            return nullptr;
        }

        Model::Ref model = memory::CreateRef< Model >();

        if ( pAiScene->HasMeshes() )
        {
            for ( Index m{}; m < pAiScene->mNumMeshes; ++m )
            {
                const aiMesh &aiMesh = *pAiScene->mMeshes[m];

                if ( !aiMesh.HasBones() )
                {
                    auto mesh = LoadMesh( aiMesh );
                    model->m_Meshes.PushBack( std::move( mesh ) );
                }
                else
                {
                    auto pSkinnedMeshFilter = LoadSkinnedMesh( aiMesh );
                    model->m_SkinnedMeshes.PushBack( pSkinnedMeshFilter );
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
                    AnimationClip animClip = LoadAnimation( model, *pAiAnim, *pAiScene );
                    model->m_AnimationClips.PushBack( std::move(animClip) );
                }
            }
        }

        aiReleaseImport( pAiScene );
        aiReleasePropertyStore( pPropertyStore );

        return model;
    }

    MeshSource ModelLoader::LoadMesh( const aiMesh &aiMesh )
    {
        MeshSource meshSource{ aiMesh.mName.C_Str() };

        LoadVertices( meshSource, aiMesh );

        return meshSource;
    }

    SkinnedMeshSource ModelLoader::LoadSkinnedMesh( const aiMesh &aiMesh )
    {
        SkinnedMeshSource skinnedMeshSource{ aiMesh.mName.C_Str() };

        skinnedMeshSource.m_Semantics.Set( graphic::Semantic::BlendIndices );
        skinnedMeshSource.m_Semantics.Set( graphic::Semantic::BlendWeights );

        skinnedMeshSource.m_BlendIndices.SetItemCount( aiMesh.mNumVertices );
        skinnedMeshSource.m_BlendWeights.SetItemCount( aiMesh.mNumVertices );
        std::fill( skinnedMeshSource.m_BlendIndices.begin(),
            skinnedMeshSource.m_BlendIndices.end(),
            DirectX::XMFLOAT4{ -1, -1, -1, -1 } );

        LoadVertices( skinnedMeshSource, aiMesh );
        LoadBones( skinnedMeshSource, aiMesh );

        return skinnedMeshSource;
    }

    void ModelLoader::LoadVertices( MeshSource &mesh, const aiMesh &aiMesh )
    {
        mesh.SetVertexCount( aiMesh.mNumVertices );

        for ( Uint32 v{}; v < aiMesh.mNumVertices; ++v )
        {
            const aiVector3D &vertex = aiMesh.mVertices[v];
            auto position = utils::ConvertToVector3( vertex );
            mesh.AddPosition( position );

            if ( aiMesh.HasNormals() )
            {
                const aiVector3D &normal = aiMesh.mNormals[v];
                mesh.AddNormal( utils::ConvertToVector3( normal ) );
            }

            if ( aiMesh.HasTextureCoords( 0 ) )
            {
                const aiVector3D &texCoord = aiMesh.mTextureCoords[0][v];
                mesh.AddTexCoord( { texCoord.x, 1 - texCoord.y } );
            }

            if ( aiMesh.HasTangentsAndBitangents() )
            {
                const aiVector3D &tangent = aiMesh.mTangents[v];
                mesh.AddTangent( utils::ConvertToVector3( tangent ) );
            }

            if ( aiMesh.HasVertexColors( 0 ) )
            {
                const aiColor4D &color = aiMesh.mColors[0][v];
                mesh.AddColor( utils::ConvertToVector4( color ) );
            }
        }

        mesh.SetIndexCount( aiMesh.mNumFaces * 3 );

        for ( Index f{}; f < aiMesh.mNumFaces; ++f )
        {
            const Index index0 = aiMesh.mFaces[f].mIndices[0];
            const Index index1 = aiMesh.mFaces[f].mIndices[1];
            const Index index2 = aiMesh.mFaces[f].mIndices[2];

            const Index indexIdx = f * 3;
            mesh.AddIndex( indexIdx, index0 );
            mesh.AddIndex( indexIdx + 1, index1 );
            mesh.AddIndex( indexIdx + 2, index2 );
        }
    }

    void ModelLoader::LoadBones( SkinnedMeshSource &mesh, const aiMesh &aiMesh )
    {
        for ( Index i{}; i < aiMesh.mNumBones; ++i )
        {
            aiBone *pBone = aiMesh.mBones[i];
            if ( pBone )
            {
                Index boneID = s_InvalidIndex;
                primitive::String boneName{ pBone->mName.C_Str() };

                if ( mesh.m_SkeletonMap.HasItemAtKey( boneName ) )
                {
                    graphic::BoneInfo boneInfo{};
                    boneInfo.ID = mesh.m_BoneCount;
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

                    mesh.m_SkeletonMap[boneName] = boneInfo;
                    boneID = mesh.m_BoneCount;
                    ++mesh.m_BoneCount;
                }
                else
                {
                    boneID = mesh.m_SkeletonMap[boneName].ID;
                }

                SM_ASSERT_MSG( boneID != -1, "ModelLoader::LoadBones > Invalid bone ID" );

                for ( Index j{}; j < pBone->mNumWeights; ++j )
                {
                    aiVertexWeight &weight = pBone->mWeights[j];
                    if ( &weight )
                    {
                        SM_ASSERT_MSG(
                            weight.mVertexId <= mesh.m_VertexCount, "ModelLoader::LoadBones > Invalid vertex ID" );

                        if ( mesh.m_BlendIndices[weight.mVertexId].x < 0 )
                        {
                            mesh.m_BlendIndices[weight.mVertexId].x = static_cast< float >( boneID );
                            mesh.m_BlendWeights[weight.mVertexId].x = weight.mWeight;
                        }
                        else if ( mesh.m_BlendIndices[weight.mVertexId].y < 0 )
                        {
                            mesh.m_BlendIndices[weight.mVertexId].y = static_cast< float >( boneID );
                            mesh.m_BlendWeights[weight.mVertexId].y = weight.mWeight;
                        }
                        else if ( mesh.m_BlendIndices[weight.mVertexId].z < 0 )
                        {
                            mesh.m_BlendIndices[weight.mVertexId].z = static_cast< float >( boneID );
                            mesh.m_BlendWeights[weight.mVertexId].z = weight.mWeight;
                        }
                        else if ( mesh.m_BlendIndices[weight.mVertexId].w < 0 )
                        {
                            mesh.m_BlendIndices[weight.mVertexId].w = static_cast< float >( boneID );
                            mesh.m_BlendWeights[weight.mVertexId].w = weight.mWeight;
                        }
                    }
                }
            }
        }
    }

    AnimationClip ModelLoader::LoadAnimation( Model::ConstRef model, const aiAnimation &aiAnim, const aiScene &aiScene )
    {
        AnimationClip animClip{ aiAnim.mName.C_Str(),
            static_cast< float >( aiAnim.mDuration ),
            static_cast< float >( aiAnim.mTicksPerSecond ) };

        LoadNodeHierarchy( animClip.RootNode, *aiScene.mRootNode );

        for ( Uint32 j{}; j < aiAnim.mNumChannels; ++j )
        {
            aiNodeAnim *pChannel = aiAnim.mChannels[j];
            if ( pChannel )
            {
                primitive::String boneName{ pChannel->mNodeName.C_Str() };

                for ( auto &skinnedMesh : model->m_SkinnedMeshes )
                {
                    auto boneInfoMapIt = skinnedMesh.m_SkeletonMap.FindItemAtKey( boneName );
                    if ( boneInfoMapIt != skinnedMesh.m_SkeletonMap.end() )
                    {
                        const BoneInfo &boneInfo = ( *boneInfoMapIt ).Value;
                        Bone bone{ boneName, boneInfo.ID };

                        bone.m_TranslationCount = pChannel->mNumPositionKeys;
                        for ( Index k{}; k < pChannel->mNumPositionKeys; ++k )
                        {
                            const DirectX::XMFLOAT3 translation =
                                *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mPositionKeys[k].mValue );

                            const float tick = static_cast< float >( pChannel->mPositionKeys[k].mTime );

                            KeyTranslation keyTranslation{ translation, tick };
                            bone.m_Translations.PushBack( std::move( keyTranslation ) );
                        }

                        bone.m_RotationCount = pChannel->mNumRotationKeys;
                        for ( Index k{}; k < pChannel->mNumRotationKeys; ++k )
                        {
                            const auto rotation = DirectX::XMFLOAT4{ pChannel->mRotationKeys[k].mValue.x,
                                pChannel->mRotationKeys[k].mValue.y,
                                pChannel->mRotationKeys[k].mValue.z,
                                pChannel->mRotationKeys[k].mValue.w };

                            const float tick = static_cast< float >( pChannel->mRotationKeys[k].mTime );

                            KeyRotation keyRotation{ rotation, tick };
                            bone.m_Rotations.PushBack( std::move( keyRotation ) );
                        }

                        bone.m_ScaleCount = pChannel->mNumScalingKeys;
                        for ( Index k{}; k < pChannel->mNumScalingKeys; ++k )
                        {
                            const auto scale =
                                *reinterpret_cast< DirectX::XMFLOAT3 * >( &pChannel->mScalingKeys[k].mValue );

                            const float tick = static_cast< float >( pChannel->mScalingKeys[k].mTime );

                            KeyScale keyScale{ scale, tick };
                            bone.m_Scales.PushBack( std::move( keyScale ) );
                        }

                        animClip.Bones.PushBack( std::move( bone ) );
                        break;
                    }
                }
            }
        }

        return animClip;
    }

    void ModelLoader::LoadNodeHierarchy( AnimationNode &dest, const aiNode &src )
    {
        dest.Name = src.mName.data;
        // if (src->mNumMeshes > 0)
        {
            dest.Transform = DirectX::XMFLOAT4X4{ src.mTransformation.a1,
                src.mTransformation.b1,
                src.mTransformation.c1,
                src.mTransformation.d1,
                src.mTransformation.a2,
                src.mTransformation.b2,
                src.mTransformation.c2,
                src.mTransformation.d2,
                src.mTransformation.a3,
                src.mTransformation.b3,
                src.mTransformation.c3,
                src.mTransformation.d3,
                src.mTransformation.a4,
                src.mTransformation.b4,
                src.mTransformation.c4,
                src.mTransformation.d4 };
        }
        /*else
        {
            DirectX::XMStoreFloat4x4(&dest.Transform, DirectX::XMMatrixIdentity());
        }*/

        dest.ChildrenCount = src.mNumChildren;

        for ( Index i{}; i < src.mNumChildren; ++i )
        {
            AnimationNode newNode{};
            LoadNodeHierarchy( newNode, *src.mChildren[i] );
            dest.Children.PushBack( std::move( newNode ) );
        }
    }
}