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
        descriptor.WorkingDirectory = "../smile-editor";
        descriptor.CommandLineArgs = commandLineArgs;

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new SmileEditor{ descriptor };
    }
}