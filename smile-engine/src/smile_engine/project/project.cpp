#include "smpch.h"
#include "project.h"

#include "project_serializer.h"

namespace smile::project
{
    Ref< Project > Project::New()
    {
        s_pActiveProject = CreateRef< Project >();
        return s_pActiveProject;
    }

    Ref< Project > Project::Load( const std::filesystem::path &path )
    {
        Ref< Project > pProject = CreateRef< Project >();
        ProjectSerializer serializer{ pProject };

        if ( serializer.Deserialize( path ) )
        {
            s_pActiveProject = pProject;
            return s_pActiveProject;
        }
        else
        {
            return nullptr;
        }
    }

    bool Project::SaveActive( const std::filesystem::path &path )
    {
        ProjectSerializer serializer{ s_pActiveProject };
        return serializer.Serialize( path );
    }
}