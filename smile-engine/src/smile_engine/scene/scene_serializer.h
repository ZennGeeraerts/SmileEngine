#pragma once

#include "scene.h"

namespace smile
{
    class SceneSerializer final
    {
     public:
        SceneSerializer( const Ref< Scene > &pScene );

        void Serialize( const std::string &filePath );
        // Serializes the scene to a binary format
        void SerializeRuntime( const std::string &filePath );

        bool Deserialize( const std::string &filePath );
        // Deserializes the scene from a binary format
        bool DeserializeRuntime( const std::string &filePath );

     private:
        Ref< Scene > m_pScene = nullptr;
    };
}
