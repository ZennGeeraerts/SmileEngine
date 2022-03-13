#include "smile_engine.h"
#include "smile_engine/core/entry_point.h"

#include "smile_editor_layer.h"

namespace smile
{
    class SmileEditor final : public Application
    {
      public:
        SmileEditor() : Application( "Smile Editor" )
        {
            PushLayer( new SmileEditorLayer{} );
        }

        ~SmileEditor()
        {
        }
    };

    Application *CreateGame()
    {
        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new SmileEditor{};
    }
}