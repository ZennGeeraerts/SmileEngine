#pragma once
#include "Core.h"

namespace Smile
{
	class SMILE_API SmileGame
	{
	public:
		SmileGame();
		virtual ~SmileGame();

		void Run();
	};

	// To be defined in client
	SmileGame* CreateGame();
}

