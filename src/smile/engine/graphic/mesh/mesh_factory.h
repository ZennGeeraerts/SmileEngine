/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh.h"
#include "skinned_mesh.h"

#include "mesh_filter.h"
#include "skinned_mesh_filter.h"
#include "engine/graphic/renderer_backend/shader/buffer.h"

namespace smile::graphic
{
    class MeshFactory final
    {
      public:
        static Ref< Mesh > CreateMesh( const Ref< MeshFilter > &pMeshFilter, const BufferLayout &layout );
        static Ref< SkinnedMesh > CreateSkinnedMesh( const Ref< SkinnedMeshFilter > &pSkinnedMeshFilter,
            const BufferLayout &layout );

        static Ref< Mesh > CreatePlane( const BufferLayout &bufferLayout );
        static Ref< Mesh > CreateCube( const BufferLayout &bufferLayout );
        static Ref< Mesh > CreateSphere( const BufferLayout &bufferLayout, const float radius, const Uint32 steps );

      private:
        static const DirectX::XMFLOAT4 s_DefaultColor;
        static const DirectX::XMFLOAT4 s_DefaultFloat4;
        static const DirectX::XMFLOAT3 s_DefaultFloat3;
        static const DirectX::XMFLOAT2 s_DefaultFloat2;
        static const DirectX::XMFLOAT4 s_DefaultIndices4;
    };
}