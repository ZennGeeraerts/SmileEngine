/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile.h"
#include "smile/core/application/entry_point.h"
#include "smile/blueprint/interactive/interactive_application.h"

#include "smile_editor_layer.h"

namespace smile
{
    class SmileEditor final : public interactive::InteractiveApplication
    {
      public:
        SmileEditor( const application::ApplicationDescriptor &descriptor )
            : interactive::InteractiveApplication{ descriptor }
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