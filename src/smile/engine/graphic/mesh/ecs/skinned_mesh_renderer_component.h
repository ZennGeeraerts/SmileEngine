/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer/render_engine.h"
#include "engine/graphic/renderer/resource_manager.h"
#include "engine/graphic/mesh/model_loader.h"
#include "engine/graphic/mesh/mesh_factory.h"
#include "engine/graphic/mesh/material.h"

namespace smile::graphic::ecs
{
    struct SkinnedMeshRendererComponent final
    {
        SkinnedMeshRendererComponent()
        {
            // TODO: Get shader from shader library in render engine
            graphic::ResourceManager &resourceManager = graphic::RenderEngine::GetRenderSystem().GetResourceManager();
            auto pShader = resourceManager.CreateShader( "resources/shaders/PBR_Skinned.fx" );
            pMaterial = smile::CreateRef< graphic::Material >( pShader );
        }

        SkinnedMeshRendererComponent( const SkinnedMeshRendererComponent & ) = default;

        SkinnedMeshRendererComponent( const std::string &assetFile,
            Uint32 meshIndex,
            const Ref< graphic::Material > &pMaterial )
            : pMaterial{ pMaterial }, MeshIndex{ meshIndex }
        {
            pModel = graphic::ModelLoader::LoadModel( assetFile );
            Ref< graphic::SkinnedMeshFilter > pSkinnedMeshFilter = pModel->GetSkinnedMeshFilter( meshIndex );
            pSkinnedMesh = graphic::MeshFactory::CreateSkinnedMesh( pSkinnedMeshFilter, pMaterial->GetBufferLayout() );
        }

        Ref< graphic::SkinnedMesh > pSkinnedMesh = nullptr;
        Ref< graphic::Material > pMaterial = nullptr;

        // For serialization
        Ref< graphic::Model > pModel = nullptr;
        Uint32 MeshIndex = 0;
    };
}