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
            pushLayer( new SmileEditorLayer{} );
        }

        ~SmileEditor()
        {
        }
    };

    Application *createApplication( ApplicationCommandLineArgs command_line_args )
    {
        ApplicationDescriptor descriptor{};
        descriptor.name = "Smile Editor";
        descriptor.workingDirectory = "../smile-editor";
        descriptor.commandLineArgs = command_line_args;

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new SmileEditor{ descriptor };
    }
}