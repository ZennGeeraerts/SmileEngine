/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "world.h"

#include <filesystem>

namespace smile::world
{
    class WorldManager final
    {
      public:
        static Ref< World > GetActive()
        {
            return s_pActiveWorld;
        }

        static Ref< World > New();
        static Ref< World > Load( const std::filesystem::path &path );
        static void Open( const Ref< World > &pWorld );
        static void SaveActive( const std::filesystem::path &path );

      private:
        inline static Ref< World > s_pActiveWorld;
    };
}