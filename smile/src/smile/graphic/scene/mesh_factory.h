/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh.h"
#include "skinned_mesh.h"

#include "mesh_source.h"
#include "skinned_mesh_source.h"
#include "smile/graphic/rhi/resource/buffer.h"
#include "smile/graphic/renderer/resource/resource_manager.h"

namespace smile::graphic
{
    class MeshFactory final
    {
      public:
        MeshFactory( ResourceManager &resourceManager ) noexcept : m_ResourceManager{ resourceManager }
        {
        }

        Mesh CreateMesh( const MeshSource::Ref &meshSource, const rhi::BufferLayout &layout ) const;
        SkinnedMesh CreateSkinnedMesh( const SkinnedMeshSource::Ref &skinnedMeshSource, const rhi::BufferLayout &layout ) const;

        Mesh CreatePlane( const rhi::BufferLayout &vertexLayout ) const;
        Mesh CreateCube( const rhi::BufferLayout &vertexLayout ) const;
        Mesh CreateSphere( const rhi::BufferLayout &vertexLayout, const float radius, const Count steps ) const;

      private:
        static const DirectX::XMFLOAT4 s_DefaultColor;
        static const DirectX::XMFLOAT4 s_DefaultFloat4;
        static const DirectX::XMFLOAT3 s_DefaultFloat3;
        static const DirectX::XMFLOAT2 s_DefaultFloat2;
        static const DirectX::XMFLOAT4 s_DefaultIndices4;

        ResourceManager &m_ResourceManager;
    };
}