/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "world.h"
#include "smile/common/memory/ref.h"
#include "smile/common/primitive/collection/hash_map.h"

#include <filesystem>

namespace smile::world
{
    class WorldSerializer final
    {
      public:
        WorldSerializer( memory::Ref< World > pWorld );

        void Serialize( const std::filesystem::path &filePath );
        // Serializes the world to a binary format
        void SerializeRuntime( const std::filesystem::path &filePath );

        bool Deserialize( const std::filesystem::path &filePath );
        // Deserializes the world from a binary format
        bool DeserializeRuntime( const std::filesystem::path &filePath );

      private:
        memory::Ref< World > m_pWorld = nullptr;
    };
}
