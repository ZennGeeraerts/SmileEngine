#include <SmileEngine.h>
#include <SmileEngine/Core/EntryPoint.h>

#include "SmileEditorLayer.h"

namespace Smile
{
	class SmileEditor final : public Application
	{
	public:
		SmileEditor()
			: Application("Smile Editor")
		{
			PushLayer(new SmileEditorLayer{});
		}

		~SmileEditor()
		{

		}
	};

	Application* CreateGame()
	{
		// This application will get passed to the entry point of the engine
		// and will be deleted once the engine closes
		return new SmileEditor{};
	}
}