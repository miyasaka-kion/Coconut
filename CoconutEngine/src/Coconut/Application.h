#pragma once

#include "Core.h"

namespace Coconut {

	class CC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};
	
	// This will be defined in the client
	Application* CreateApplication();
}



