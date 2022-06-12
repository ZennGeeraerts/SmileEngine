#include "smpch.h"
#include "mesh_loader.h"

#include "smile_engine/core/logger.h"
#include "mesh_loader_utils.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace smile::renderer
{
    std::vector< Ref< StaticMeshFilter > > MeshLoader::loadStaticMesh( const std::string &file_path )
    {
        aiPropertyStore *property_store = aiCreatePropertyStore();
        aiSetImportPropertyInteger( property_store, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0 );

        const aiScene *ai_scene = aiImportFileExWithProperties(
            file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality, nullptr, property_store );
        if ( !ai_scene )
        {
            SM_LOG_WARNING( "MeshLoader::loadStaticMesh > Could not load file: %s: %s", file_path, aiGetErrorString() );
            aiReleaseImport( ai_scene );
            aiReleasePropertyStore( property_store );
            return std::vector< Ref< StaticMeshFilter > >{};
        }

        std::vector< Ref< StaticMeshFilter > > static_meshes{};
        static_meshes.resize( ai_scene->mNumMeshes );

        for ( Uint32 m{}; m < ai_scene->mNumMeshes; ++m )
        {
            aiMesh *ai_mesh = ai_scene->mMeshes[m];
            static_meshes[m].reset( new StaticMeshFilter{} );

            loadVertices( static_meshes[m], ai_mesh );
            static_meshes[m]->filePath = file_path;
        }

        aiReleaseImport( ai_scene );
        aiReleasePropertyStore( property_store );

        return static_meshes;
    }

    std::vector< Ref< SkinnedMeshFilter > > MeshLoader::loadSkinnedMesh( const std::string &file_path )
    {
        aiPropertyStore *property_store = aiCreatePropertyStore();
        aiSetImportPropertyInteger( property_store, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0 );

        const aiScene *ai_scene = aiImportFileExWithProperties(
            file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality, nullptr, property_store );
        if ( !ai_scene )
        {
            SM_LOG_WARNING( "MeshLoader::LoadSkinnedMesh > Could not load file: %s: %s", file_path, aiGetErrorString() );
            aiReleaseImport( ai_scene );
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

        std::vector< Ref< SkinnedMeshFilter > > skinned_meshes{};
        skinned_meshes.resize( ai_scene->mNumMeshes );

        for ( Uint32 m{}; m < ai_scene->mNumMeshes; ++m )
        {
            aiMesh *ai_mesh = ai_scene->mMeshes[m];
            skinned_meshes[m].reset( new SkinnedMeshFilter{} );

            loadVertices( skinned_meshes[m], ai_mesh );

            if ( ai_mesh->HasBones() )
            {
                skinned_meshes[m]->useBlendIndices = true;
                skinned_meshes[m]->useBlendWeights = true;

                skinned_meshes[m]->blendIndices.resize( ai_mesh->mNumVertices );
                skinned_meshes[m]->blendWeights.resize( ai_mesh->mNumVertices );
                std::fill( skinned_meshes[m]->blendIndices.begin(),
                    skinned_meshes[m]->blendIndices.end(),
                    DirectX::XMFLOAT4{ -1, -1, -1, -1 } );

                loadBones( skinned_meshes[m], ai_mesh, ai_scene );
            }

            skinned_meshes[m]->filePath = file_path;
        }

        if ( ai_scene->HasAnimations() )
            loadAnimations( skinned_meshes[0], ai_scene );

        aiReleaseImport( ai_scene );
        aiReleasePropertyStore( property_store );

        return skinned_meshes;
    }

    void MeshLoader::loadVertices( const Ref< MeshFilter > &mesh, aiMesh *ai_mesh )
    {
        mesh->setVertexCount( ai_mesh->mNumVertices );

        for ( Uint32 v{}; v < ai_mesh->mNumVertices; ++v )
        {
            aiVector3D &vertex = ai_mesh->mVertices[v];
            mesh->addPosition( utils::convertToVector3( vertex ) );

            if ( ai_mesh->HasNormals() )
            {
                aiVector3D &normal = ai_mesh->mNormals[v];
                mesh->addNormal( utils::convertToVector3( normal ) );
            }

            if ( ai_mesh->HasTextureCoords( 0 ) )
            {
                aiVector3D &tex_coord = ai_mesh->mTextureCoords[0][v];
                mesh->addTexCoord( { tex_coord.x, 1 - tex_coord.y } );
            }

            if ( ai_mesh->HasTangentsAndBitangents() )
            {
                aiVector3D &tangent = ai_mesh->mTangents[v];
                mesh->addTangent( utils::convertToVector3( tangent ) );
            }

            if ( ai_mesh->HasVertexColors( 0 ) )
            {
                aiColor4D &color = ai_mesh->mColors[0][v];
                mesh->addColor( utils::convertToVector4( color ) );
            }
        }

        mesh->setIndexCount( ai_mesh->mNumFaces * 3 );

        for ( Uint32 f{}; f < ai_mesh->mNumFaces; ++f )
        {
            Uint32 index0 = ai_mesh->mFaces[f].mIndices[0];
            Uint32 index1 = ai_mesh->mFaces[f].mIndices[1];
            Uint32 index2 = ai_mesh->mFaces[f].mIndices[2];

            Uint32 index_idx = static_cast< size_t >( f ) * 3;
            mesh->addIndex( index_idx, index0 );
            mesh->addIndex( index_idx + 1, index1 );
            mesh->addIndex( index_idx + 2, index2 );
        }
    }

    void MeshLoader::loadBones( const Ref< SkinnedMeshFilter > &mesh, aiMesh *ai_mesh, const aiScene *ai_scene )
    {
        for ( Uint32 i{}; i < ai_mesh->mNumBones; ++i )
        {
            aiBone *bone = ai_mesh->mBones[i];
            if ( bone )
            {
                Uint32 bone_id = -1;
                std::string boneName = bone->mName.C_Str();

                if ( mesh->skeletonMap.find( boneName ) == mesh->skeletonMap.end() )
                {
                    BoneInfo bone_info{};
                    bone_info.id = mesh->boneCount;
                    bone_info.offset = DirectX::XMFLOAT4X4{ bone->mOffsetMatrix.a1,
                        bone->mOffsetMatrix.b1,
                        bone->mOffsetMatrix.c1,
                        bone->mOffsetMatrix.d1,
                        bone->mOffsetMatrix.a2,
                        bone->mOffsetMatrix.b2,
                        bone->mOffsetMatrix.c2,
                        bone->mOffsetMatrix.d2,
                        bone->mOffsetMatrix.a3,
                        bone->mOffsetMatrix.b3,
                        bone->mOffsetMatrix.c3,
                        bone->mOffsetMatrix.d3,
                        bone->mOffsetMatrix.a4,
                        bone->mOffsetMatrix.b4,
                        bone->mOffsetMatrix.c4,
                        bone->mOffsetMatrix.d4 };

                    mesh->skeletonMap[boneName] = bone_info;
                    bone_id = mesh->boneCount;
                    ++mesh->boneCount;
                }
                else
                {
                    bone_id = mesh->skeletonMap[boneName].id;
                }

                SM_ASSERT( bone_id != -1, "MeshLoader::loadBones > Invalid bone ID" );

                for ( Uint32 j{}; j < bone->mNumWeights; ++j )
                {
                    aiVertexWeight &weight = bone->mWeights[j];
                    if ( &weight )
                    {
                        SM_ASSERT(
                            weight.mVertexId <= mesh->vertexCount, "MeshLoader::loadBones > Invalid vertex ID" );

                        if ( mesh->blendIndices[weight.mVertexId].x < 0 )
                        {
                            mesh->blendIndices[weight.mVertexId].x = static_cast< float >( bone_id );
                            mesh->blendWeights[weight.mVertexId].x = weight.mWeight;
                        }
                        else if ( mesh->blendIndices[weight.mVertexId].y < 0 )
                        {
                            mesh->blendIndices[weight.mVertexId].y = static_cast< float >( bone_id );
                            mesh->blendWeights[weight.mVertexId].y = weight.mWeight;
                        }
                        else if ( mesh->blendIndices[weight.mVertexId].z < 0 )
                        {
                            mesh->blendIndices[weight.mVertexId].z = static_cast< float >( bone_id );
                            mesh->blendWeights[weight.mVertexId].z = weight.mWeight;
                        }
                        else if ( mesh->blendIndices[weight.mVertexId].w < 0 )
                        {
                            mesh->blendIndices[weight.mVertexId].w = static_cast< float >( bone_id );
                            mesh->blendWeights[weight.mVertexId].w = weight.mWeight;
                        }
                    }
                }
            }
        }
    }

    void MeshLoader::loadAnimations( const Ref< SkinnedMeshFilter > &mesh, const aiScene *ai_scene )
    {
        mesh->animated = true;

        for ( Uint32 i{}; i < ai_scene->mNumAnimations; ++i )
        {
            aiAnimation *anim = ai_scene->mAnimations[i];
            if ( anim )
            {
                AnimationClip anim_clip{};
                anim_clip.name = anim->mName.C_Str();
                anim_clip.duration = static_cast< float >( anim->mDuration );
                anim_clip.ticksPerSecond = static_cast< float >( anim->mTicksPerSecond );

                loadNodeHierarchy( anim_clip.rootNode, ai_scene->mRootNode );

                for ( Uint32 j{}; j < anim->mNumChannels; ++j )
                {
                    aiNodeAnim *channel = anim->mChannels[j];
                    if ( channel )
                    {
                        std::string bone_name = channel->mNodeName.C_Str();
                        auto bone_info_map_it = mesh->skeletonMap.find( bone_name );
                        if ( bone_info_map_it != mesh->skeletonMap.end() )
                        {
                            BoneInfo &bone_info = ( *bone_info_map_it ).second;
                            Bone bone{ bone_name, bone_info.id };

                            bone.translationCount = channel->mNumPositionKeys;
                            for ( Uint32 k{}; k < channel->mNumPositionKeys; ++k )
                            {
                                KeyTranslation key_translation{};
                                key_translation.translation =
                                    *reinterpret_cast< DirectX::XMFLOAT3 * >( &channel->mPositionKeys[k].mValue );
                                key_translation.tick = static_cast< float >( channel->mPositionKeys[k].mTime );
                                bone.translations.push_back( key_translation );
                            }

                            bone.rotationCount = channel->mNumRotationKeys;
                            for ( Uint32 k{}; k < channel->mNumRotationKeys; ++k )
                            {
                                KeyRotation key_rotation{};
                                key_rotation.rotation = DirectX::XMFLOAT4{ channel->mRotationKeys[k].mValue.x,
                                    channel->mRotationKeys[k].mValue.y,
                                    channel->mRotationKeys[k].mValue.z,
                                    channel->mRotationKeys[k].mValue.w };
                                key_rotation.tick = static_cast< float >( channel->mRotationKeys[k].mTime );
                                bone.rotations.push_back( key_rotation );
                            }

                            bone.scaleCount = channel->mNumScalingKeys;
                            for ( Uint32 k{}; k < channel->mNumScalingKeys; ++k )
                            {
                                KeyScale key_scale{};
                                key_scale.scale =
                                    *reinterpret_cast< DirectX::XMFLOAT3 * >( &channel->mScalingKeys[k].mValue );
                                key_scale.tick = static_cast< float >( channel->mScalingKeys[k].mTime );
                                bone.scales.push_back( key_scale );
                            }

                            anim_clip.bones.push_back( bone );
                        }
                    }
                }

                mesh->animationClips.push_back( anim_clip );
            }
        }
    }

    void MeshLoader::loadNodeHierarchy( AnimationNode &dest, const aiNode *src )
    {
        dest.name = src->mName.data;
        // if (src->mNumMeshes > 0)
        {
            dest.transform = DirectX::XMFLOAT4X4{ 
                src->mTransformation.a1,
                src->mTransformation.b1,
                src->mTransformation.c1,
                src->mTransformation.d1,
                src->mTransformation.a2,
                src->mTransformation.b2,
                src->mTransformation.c2,
                src->mTransformation.d2,
                src->mTransformation.a3,
                src->mTransformation.b3,
                src->mTransformation.c3,
                src->mTransformation.d3,
                src->mTransformation.a4,
                src->mTransformation.b4,
                src->mTransformation.c4,
                src->mTransformation.d4 };
        }
        /*else
        {
            DirectX::XMStoreFloat4x4(&dest.Transform, DirectX::XMMatrixIdentity());
        }*/

        dest.childrenCount = src->mNumChildren;

        for ( Uint32 i{}; i < src->mNumChildren; ++i )
        {
            AnimationNode new_node{};
            loadNodeHierarchy( new_node, src->mChildren[i] );
            dest.children.push_back( new_node );
        }
    }
}