#pragma once

#include "project.h"

namespace smile::project
{
    class ProjectSerializer final
    {
      public:
        ProjectSerializer( Ref< Project > pProject );

        bool Serialize( const std::filesystem::path &filePath );
        bool Deserialize( const std::filesystem::path &filePath );

      private:
        Ref< Project > m_pContext;
    };
}