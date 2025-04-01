/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "world.h"
#include "world_loader.h"

#include <filesystem>

namespace smile::world
{
    class WorldManager final
    {
      public:
        class Listener
        {
          public:
            virtual void OnWorldOpened( smile::ecs::ECSEngine &ecsEngine ) = 0;
            virtual void OnWorldClosed( smile::ecs::ECSEngine &ecsEngine ) = 0;
        };

      public:
        static memory::Ref< World > GetActive()
        {
            return s_pActiveWorld;
        }

        static memory::Ref< World > New();
        static memory::Ref< World > Load( const std::filesystem::path &path );
        static void UnloadActive();
        static void Open( memory::Ref< World > pWorld );
        static void SaveActive( const std::filesystem::path &path );
        static memory::Ref< World > CopyActive();

        static void AddListener( Listener *pListener );

      private:
        inline static memory::Ref< World > s_pActiveWorld;
        inline static WorldLoader s_WorldLoader;

        inline static std::vector< Listener * > s_pListeners;
    };
}