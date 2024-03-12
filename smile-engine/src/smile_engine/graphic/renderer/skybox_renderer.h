/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/camera/camera.h"
#include "smile_engine/graphic/camera/editor_camera.h"
#include "smile_engine/graphic/mesh/mesh.h"
#include "smile_engine/graphic/shader/shader.h"

namespace smile::graphic
{
    class SkyboxRenderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void BeginScene( const EditorCamera &editorCamera );
        static void OnRender();
        static void EndScene();

      private:
        static Ref< Mesh > s_pCubeMesh;
        static Ref< Shader > s_pSkyboxShader;
    };
}