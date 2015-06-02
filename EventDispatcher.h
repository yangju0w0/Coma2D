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
		const char* type;
		EventFunction function;
	};
	class EventDispatcher
	{
	public:
		EventDispatcher();
		virtual ~EventDispatcher();
		
		void setEventListener(const char* type, EventFunction function);
		void removeEventListener(const char* type, EventFunction function);
		bool hasEventListener(const char* type);
		void dispatchEvent(Event* event);
	private:
		std::vector<Listener> listenerList;
	};
}