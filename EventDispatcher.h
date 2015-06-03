#pragma once
#include <functional>
#include <vector>
#include "Event.h"
#define BIND(T) std::bind(&T, this, std::placeholders::_1)

namespace Coma2D
{
	typedef std::function<void(Event*)> EventFunction;
	
	struct Listener
	{
		std::string type;
		EventFunction function;
	};
	class EventDispatcher
	{
	public:
		EventDispatcher();
		virtual ~EventDispatcher();
		
		void setEventListener(std::string type, EventFunction function);
		void removeEventListener(std::string type, EventFunction function);
		bool hasEventListener(std::string type);
		void dispatchEvent(Event* event);
	private:
		std::vector<Listener> listenerList;
	};
}