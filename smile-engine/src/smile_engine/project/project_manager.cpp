/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "project_manager.h"

#include "project_serializer.h"

namespace smile::project
{
    Ref< Project > ProjectManager::New()
    {
        s_pActiveProject = CreateRef< Project >();
        return s_pActiveProject;
    }

    Ref< Project > ProjectManager::Load( const std::filesystem::path &path )
    {
        Ref< Project > pProject = CreateRef< Project >();
        ProjectSerializer serializer{ pProject };

        if ( serializer.Deserialize( path ) )
        {
            pProject->m_ProjectDirectory = path.parent_path();
            s_pActiveProject = pProject;
            return s_pActiveProject;
        }
        else
        {
            return nullptr;
        }
    }

    bool ProjectManager::SaveActive( const std::filesystem::path &path )
    {
        ProjectSerializer serializer{ s_pActiveProject };
        if ( serializer.Serialize( path ) )
        {
            s_pActiveProject->m_ProjectDirectory = path.parent_path();
            return true;
        }
        else
        {
            return false;
        }
    }
}