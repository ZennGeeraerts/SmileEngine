#pragma once
#include "static_mesh_filter.h"
#include "skinned_mesh_filter.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace smile::renderer
{
    class MeshLoader final
    {
      public:
        static std::vector< Ref< StaticMeshFilter > > loadStaticMesh( const std::string &file_path );
        static std::vector< Ref< SkinnedMeshFilter > > loadSkinnedMesh( const std::string &file_path );

      private:
        static void loadVertices( const Ref< MeshFilter > &mesh, aiMesh *aiMesh );
        static void loadBones( const Ref< SkinnedMeshFilter > &mesh, aiMesh *aiMesh, const aiScene *aiScene );
        static void loadAnimations( const Ref< SkinnedMeshFilter > &mesh, const aiScene *aiScene );
        static void loadNodeHierarchy( AnimationNode &dest, const aiNode *src );
    };
}
