/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/render_engine.h"
#include "smile/graphic/renderer/resource/resource_manager.h"
#include "smile/graphic/scene/model_loader.h"
#include "smile/graphic/scene/mesh_factory.h"
#include "smile/graphic/scene/material.h"

namespace smile::graphic::ecs
{
    struct MeshRendererComponent final
    {
        MeshRendererComponent()
        {
            auto &shaderLibrary = RenderEngine::GetShaderLibrary();
            auto pVertexShader = shaderLibrary.GetShader( "PBR.vs.smshader" );
            auto pPixelShader = shaderLibrary.GetShader( "PBR.ps.smshader" );
            pMaterial = smile::CreateRef< graphic::Material >( pVertexShader, pPixelShader );
        }

        MeshRendererComponent( const MeshRendererComponent & ) = default;

        MeshRendererComponent( const std::string &assetFile,
            Uint32 meshIndex,
            const Ref< graphic::Material > &pMaterial )
            : pMaterial{ pMaterial }, MeshIndex{ meshIndex }
        {
            pModel = graphic::ModelLoader::LoadModel( assetFile );
            Ref< graphic::MeshSource > pMeshFilter = pModel->GetMeshFilter( meshIndex );
            pMesh = graphic::MeshFactory::CreateMesh( pMeshFilter, pMaterial->GetBufferLayout() );
        }

        Ref< graphic::Mesh > pMesh = nullptr;
        Ref< graphic::Material > pMaterial = nullptr;

        // For serialization
        Ref< graphic::Model > pModel = nullptr;
        Uint32 MeshIndex = 0;
    };
}