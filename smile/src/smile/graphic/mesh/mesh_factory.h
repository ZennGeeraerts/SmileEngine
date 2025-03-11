/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh.h"
#include "skinned_mesh.h"

#include "mesh_filter.h"
#include "skinned_mesh_filter.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"

namespace smile::graphic
{
    class MeshFactory final
    {
      public:
        static Ref< Mesh > CreateMesh( const Ref< MeshFilter > &pMeshFilter, const VertexLayout &layout );
        static Ref< SkinnedMesh > CreateSkinnedMesh( const Ref< SkinnedMeshFilter > &pSkinnedMeshFilter,
            const VertexLayout &layout );

        static Ref< Mesh > CreatePlane( const VertexLayout &vertexLayout );
        static Ref< Mesh > CreateCube( const VertexLayout &vertexLayout );
        static Ref< Mesh > CreateSphere( const VertexLayout &vertexLayout, const float radius, const Uint32 steps );

      private:
        static const DirectX::XMFLOAT4 s_DefaultColor;
        static const DirectX::XMFLOAT4 s_DefaultFloat4;
        static const DirectX::XMFLOAT3 s_DefaultFloat3;
        static const DirectX::XMFLOAT2 s_DefaultFloat2;
        static const DirectX::XMFLOAT4 s_DefaultIndices4;
    };
}