/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/asset_loader.h"
#include "model.h"

struct aiMesh;
struct aiScene;
struct aiAnimation;
struct aiNode;

namespace smile::graphic
{
    class ModelLoader final : public asset::AssetLoader
    {
      public:
        ModelLoader();

        asset::AssetType GetType() const override
        {
            return asset::AssetType{ foundation::TypeNameOf< Model >() };
        }

        const std::vector< std::filesystem::path > &GetExtensions() const override
        {
            return m_Extensions;
        }

        memory::Ref< asset::Asset > Load( asset::AssetHandle handle,
            const asset::AssetMetadata &metadata ) const override;

        Model::Ref LoadModel( const std::filesystem::path &path ) const;

      private:
        static MeshSource LoadMesh( const aiMesh &aiMesh );
        static SkinnedMeshSource LoadSkinnedMesh( const aiMesh &aiMesh );
        static void LoadVertices( MeshSource &mesh, const aiMesh &aiMesh );
        static void LoadBones( SkinnedMeshSource &mesh, const aiMesh &aiMesh );
        static AnimationClip LoadAnimation( Model::ConstRef model, const aiAnimation &aiAnim, const aiScene &aiScene );
        static void LoadNodeHierarchy( AnimationNode &dest, const aiNode &src );

        const std::vector< std::filesystem::path > m_Extensions{ ".smmat" };
    };
}
