#pragma once
#include "EventDispatcher.h"
namespace Coma2D
{
	class ComaRenderer :
		public EventDispatcher
	{
	public:
		ComaRenderer();
		virtual ~ComaRenderer();
	};
}