/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "world.h"
#include "smile/common/memory/ref.h"
#include "smile/core/fs/path.h"

namespace smile::world
{
    class WorldSerializer final
    {
      public:
        WorldSerializer( memory::Ref< World > pWorld );

        void Serialize( const fs::Path &filePath );
        // Serializes the world to a binary format
        void SerializeRuntime( const fs::Path &filePath );

        bool Deserialize( const fs::Path &filePath );
        // Deserializes the world from a binary format
        bool DeserializeRuntime( const fs::Path &filePath );

      private:
        memory::Ref< World > m_pWorld = nullptr;
    };
}
