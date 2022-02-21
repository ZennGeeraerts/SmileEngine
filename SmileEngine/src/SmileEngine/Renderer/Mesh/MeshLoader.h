#pragma once
#include "StaticMeshFilter.h"
#include "SkinnedMeshFilter.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace smile
{
    class MeshLoader final
    {
      public:
        static std::vector< Ref< StaticMeshFilter > > LoadStaticMesh( const std::string &filePath );
        static std::vector< Ref< SkinnedMeshFilter > > LoadSkinnedMesh( const std::string &filePath );

      private:
        static void LoadVertices( const Ref< MeshFilter > &pMesh, aiMesh *pAiMesh );
        static void LoadBones( const Ref< SkinnedMeshFilter > &pMesh, aiMesh *pAiMesh, const aiScene *pAiScene );
        static void LoadAnimations( const Ref< SkinnedMeshFilter > &pMesh, const aiScene *pAiScene );
        static void LoadNodeHierarchy( AnimationNode &dest, const aiNode *src );
    };
}
