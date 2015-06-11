#pragma once
#include "Event.h"
namespace coma2d
{
	class RendererEvent :
		public Event
	{
	public:
		RendererEvent(std::string type);
		virtual ~RendererEvent();
	};
}