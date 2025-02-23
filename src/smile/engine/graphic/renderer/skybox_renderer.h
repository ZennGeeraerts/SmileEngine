/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/camera/camera.h"
#include "engine/graphic/mesh/mesh.h"
#include "engine/graphic/renderer_api/shader/shader.h"

namespace smile::graphic
{
    class SkyboxRenderer final
    {
      public:
        static void Initialize();
        static void ShutDown();

        static void BeginScene( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );
        static void OnRender();
        static void EndScene();

      private:
        static Ref< Mesh > s_pCubeMesh;
        static memory::Ref< Shader > s_pSkyboxShader;
    };
}