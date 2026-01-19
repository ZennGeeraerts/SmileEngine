/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "project.h"

namespace smile::project
{
    class ProjectSerializer final
    {
      public:
        ProjectSerializer( Ref< Project > pProject );

        bool Serialize( const fs::Path &filePath );
        bool Deserialize( const fs::Path &filePath );

      private:
        Ref< Project > m_pContext;
    };
}