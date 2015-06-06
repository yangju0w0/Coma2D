#pragma once
#include "EventDispatcher.h"
namespace coma2d
{
	class ComaRenderer :
		public EventDispatcher
	{
	public:
		ComaRenderer();
		virtual ~ComaRenderer();
	};
}