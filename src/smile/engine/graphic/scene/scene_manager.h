/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "scene.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class SceneManager final
    {
      public:
        static Ref< Scene > CreateScene( const window::Window *pWindow );

      private:
        static std::vector< Ref< Scene > > s_pScenes;
    };
}