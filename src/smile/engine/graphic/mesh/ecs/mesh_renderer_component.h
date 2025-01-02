/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/engine/graphic/renderer/render_command.h"
#include "smile/engine/graphic/mesh/model_loader.h"
#include "smile/engine/graphic/mesh/mesh_factory.h"
#include "smile/engine/graphic/mesh/material.h"

namespace smile::graphic::ecs
{
    struct MeshRendererComponent final
    {
        MeshRendererComponent()
        {
            // TODO: Get shader from shader library in render engine
            graphic::GraphicsDevice *pDevice = graphic::RenderCommand::GetGraphicsDevice();
            auto pShader = pDevice->CreateShader( "assets/shaders/PBR.fx" );
            pMaterial = CreateRef< graphic::Material >( pShader );
        }

        MeshRendererComponent( const MeshRendererComponent & ) = default;

        MeshRendererComponent( const std::string &assetFile,
            Uint32 meshIndex,
            const Ref< graphic::Material > &pMaterial )
            : pMaterial{ pMaterial }, MeshIndex{ meshIndex }
        {
            pModel = graphic::ModelLoader::LoadModel( assetFile );
            Ref< graphic::MeshFilter > pMeshFilter = pModel->GetMeshFilter( meshIndex );
            pMesh = graphic::MeshFactory::CreateMesh( pMeshFilter, pMaterial->GetBufferLayout() );
        }

        Ref< graphic::Mesh > pMesh = nullptr;
        Ref< graphic::Material > pMaterial = nullptr;

        // For serialization
        Ref< graphic::Model > pModel = nullptr;
        Uint32 MeshIndex = 0;
    };
}