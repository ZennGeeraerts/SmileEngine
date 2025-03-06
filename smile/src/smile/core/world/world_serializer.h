/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "world.h"

namespace smile::world
{
    class WorldSerializer final
    {
      public:
        WorldSerializer( Ref< World > pWorld );

        void Serialize( const std::string &filePath );
        // Serializes the world to a binary format
        void SerializeRuntime( const std::string &filePath );

        bool Deserialize( const std::string &filePath );
        // Deserializes the world from a binary format
        bool DeserializeRuntime( const std::string &filePath );

      private:
        Ref< World > m_pWorld = nullptr;
    };
}
