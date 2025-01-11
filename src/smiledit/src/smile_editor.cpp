/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile_engine.h"
#include "engine/core/application/entry_point.h"

#include "smile_editor_layer.h"

namespace smile
{
    class SmileEditor final : public application::Application
    {
      public:
        SmileEditor( const application::ApplicationDescriptor &descriptor ) : Application{ descriptor }
        {
            PushLayer( new SmileEditorLayer{} );
        }

        ~SmileEditor()
        {
        }
    };

    application::Application *application::CreateApplication( application::ApplicationCommandLineArgs commandLineArgs )
    {
        application::ApplicationDescriptor descriptor{};
        descriptor.Name = "Smile Editor";

        std::filesystem::path path = std::filesystem::current_path();
        descriptor.WorkingDirectory = ".";
        descriptor.CommandLineArgs = commandLineArgs;

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new SmileEditor{ descriptor };
    }
}