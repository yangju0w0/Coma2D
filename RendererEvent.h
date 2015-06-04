#pragma once
#include "Event.h"
namespace Coma2D
{
	class RendererEvent :
		public Event
	{
	public:
		RendererEvent(std::string type);
		~RendererEvent();
	};
}