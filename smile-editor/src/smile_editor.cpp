/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile_engine.h"
#include "smile_engine/core/entry_point.h"

#include "smile_editor_layer.h"

namespace smile
{
    class SmileEditor final : public Application
    {
      public:
        SmileEditor( const ApplicationDescriptor &descriptor ) : Application{ descriptor }
        {
            PushLayer( new SmileEditorLayer{} );
        }

        ~SmileEditor()
        {
        }
    };

    Application *CreateApplication( ApplicationCommandLineArgs commandLineArgs )
    {
        ApplicationDescriptor descriptor{};
        descriptor.Name = "Smile Editor";

#ifdef SM_C_DEBUG
        descriptor.WorkingDirectory = "../../smile-editor/Debug";
#elif SM_C_RELEASE
        descriptor.WorkingDirectory = "../../smile-editor/Release";
#endif

        descriptor.CommandLineArgs = commandLineArgs;

        //project::Project::New()->SaveActive( "TestProject.smproj" );

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new SmileEditor{ descriptor };
    }
}