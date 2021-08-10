#include "smpch.h"
#include "SmileGame.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Logger.h"

namespace Smile
{
	SmileGame::SmileGame()
	{

	}

	SmileGame::~SmileGame()
	{

	}

	void SmileGame::Run()
	{
		WindowResizeEvent e{ 1280, 720 };
		SM_TRACE("Window resized: %s", e.ToString().c_str());

		while (true);
	}
}