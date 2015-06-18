#pragma once
#include "EventDispatcher.h"

namespace coma2d
{
	class Manager :
		public EventDispatcher
	{
	public:
		Manager();
		virtual ~Manager();
	};
}