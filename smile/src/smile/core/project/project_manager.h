/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "project.h"

namespace smile::project
{
    class ProjectManager final
    {
      public:
        static Ref< Project > GetActive()
        {
            return s_pActiveProject;
        }

        // TODO: move to asset manager when we have one
        static std::filesystem::path GetAssetFileSystemPath( const std::filesystem::path &assetPath )
        {
            SM_ASSERT( s_pActiveProject, "Project::GetAssetFileSystemPath > No active project" );
            return s_pActiveProject->GetAssetDirectory() / assetPath;
        }

        static Ref< Project > New();
        static Ref< Project > Load( const std::filesystem::path &path );
        static bool SaveActive( const std::filesystem::path &path );

      private:
        inline static Ref< Project > s_pActiveProject;
    };
}