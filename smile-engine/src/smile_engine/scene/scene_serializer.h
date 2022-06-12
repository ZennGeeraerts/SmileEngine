#pragma once

#include "scene.h"

namespace smile::scene
{
    class SceneSerializer final
    {
      public:
        SceneSerializer( const Ref< Scene > &scene );

        void serialize( const std::string &file_path );
        // Serializes the scene to a binary format
        void serializeRuntime( const std::string &file_path );

        bool deserialize( const std::string &file_path );
        // Deserializes the scene from a binary format
        bool deserializeRuntime( const std::string &file_path );

      private:
        Ref< Scene > scene = nullptr;
    };
}
