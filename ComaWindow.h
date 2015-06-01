#pragma once
#include "EventDispatcher.h"

namespace Coma2D
{
	class ComaWindow :
		public EventDispatcher
	{
	public:
		ComaWindow();
		~ComaWindow();
	};
}

