/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/camera.h"
#include "smile/graphic/scene/mesh.h"
#include "smile/graphic/rhi/shader/shader.h"
#include "smile/graphic/rhi/render_state.h"

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
        static RenderState s_State;
    };
}