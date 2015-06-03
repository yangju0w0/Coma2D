#pragma once
#include "Event.h"
namespace Coma2D
{
	class WindowEvent :
		public Event
	{
	public:
		WindowEvent(const char* type);
		~WindowEvent();
	};
}
