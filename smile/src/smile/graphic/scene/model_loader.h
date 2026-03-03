/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "model.h"

#include <filesystem>

struct aiMesh;
struct aiScene;
struct aiAnimation;
struct aiNode;

namespace smile::graphic
{
    class ModelLoader final
    {
      public:
        static Ref< graphic::Model > LoadModel( const std::filesystem::path &filePath );

      private:
        static Ref< graphic::MeshSource > LoadMesh( aiMesh *pAiMesh );
        static Ref< graphic::SkinnedMeshSource > LoadSkinnedMesh( aiMesh *pAiMesh );

        static void LoadVertices( const Ref< graphic::MeshSource > &pMesh, aiMesh *pAiMesh );
        static void LoadBones( const Ref< graphic::SkinnedMeshSource > &pMesh, aiMesh *pAiMesh );

        static Ref< graphic::AnimationClip >
        LoadAnimation( const Ref< graphic::Model > &pModel, const aiAnimation *pAiAnim, const aiScene *pAiScene );
        static void LoadNodeHierarchy( graphic::AnimationNode &dest, const aiNode *pSrc );
    };
}
