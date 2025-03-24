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
        class Listener
        {
          public:
            virtual void OnNewWorld( const Ref< World > &pWorld )
            {
            }
            virtual void OnActiveWorldChanged( const Ref< World > &pWorld )
            {
            }
        };

      public:
        static Ref< World > GetActive()
        {
            return s_pActiveWorld;
        }

        static Ref< World > New();
        static Ref< World > Load( const std::filesystem::path &path );
        static void UnloadActive();
        static void Open( const Ref< World > &pWorld );
        static void SaveActive( const std::filesystem::path &path );
        static Ref< World > CopyActive();

        static void AddListener( Listener *pListener );

      private:
        inline static Ref< World > s_pActiveWorld;
        inline static std::vector< Listener * > s_pListeners;
    };
}